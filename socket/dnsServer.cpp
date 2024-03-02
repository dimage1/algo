#include <cerrno>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>

#define DEFAULT_PORT 53
#define BUFFER_SIZE 1512

#define handleError(err) {printf("%s: errno=%d\n", err, errno); exit(1); }
#define handleMinorError(err) {printf("%s: errno=%d\n", err, errno);}

static void help() {
    printf("server [port]\n");
    printf("Example: server 12345\n");
}

/*
                                1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      ID                       |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    QDCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ANCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    NSCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ARCOUNT                   
*/

#pragma pack(push, 1)

struct dnsHeader {
    uint16_t id;

    union {
        struct {
            uint16_t qr : 1;
            uint16_t opCode : 4;
            uint16_t AA : 1;
            uint16_t TC : 1;
            uint16_t RD : 1;
            uint16_t RA : 1;
            uint16_t Z : 3;
            uint16_t RCODE : 4;
        } f;
        uint16_t flags;
    };
    uint16_t qdCount;
    uint16_t anCount;
    uint16_t nsCount;
    uint16_t arCount;
};
static_assert(sizeof(dnsHeader) == sizeof(uint16_t) * 6);


// 0               a standard query (QUERY)
// 1               an inverse query (IQUERY)
// 2               a server status request (STATUS)
enum {
    DNS_OPCODE_QUERY = 0,
    DNS_OPCODE_IQUERY = 1,
    DNS_OPCODE_STATUS = 2,
};

enum {
    DNS_TYPE_A = 1,
};

enum {
    DNS_QR_REQUEST = 0,
    DNS_QR_RESPONSE = 1,
};

enum {
    DNS_CLASS_IN = 1,
};

/*1  1  1  1  1  1
/0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                                               |
    /                     QNAME                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QTYPE                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QCLASS                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
*/

/*
1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                                               |
    /                                               /
    /                      NAME                     /
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      TYPE                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     CLASS                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      TTL                      |
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                   RDLENGTH                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
    /                     RDATA                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
*/

struct requestData {
    uint16_t qtype;
    uint16_t qclass;
};

struct responseAData {
    uint32_t ttl;
    uint16_t length;
    uint32_t address;
};
#pragma pack(pop)

static void printDomains(const char *domains, const char *domainsEnd)
{
    char domain[256];
    while(*domains && domains <= domainsEnd)
    {
        uint8_t len = domains[0];
        memset(domain, 0, sizeof(domain));

        // skip 1 byte len
        domains++;
        strncpy(domain, domains, (int)len);
        printf("%s.", domain);

        domains += len;
    }
    printf("\n");
}

static int processRequestAndPrepareResponse(const char* recvBuff, int recvSize, char *sendBuff, int sendMaxSize)
{
    /*for (int i = 0; i < recvSize; i++) {
        printf("%X", (int)recvBuff[i]);
    }
    printf("\n");*/

    dnsHeader header;
    if (recvSize < sizeof(dnsHeader))
    {
        printf("invalid request\n");
        return -1;
    }

    memcpy(&header, recvBuff, sizeof(header));
    header.qdCount = ntohs(header.qdCount);

    if (header.f.opCode != DNS_OPCODE_QUERY)
    {
        printf("unsupported query type: %d\n", (int)header.f.opCode);
        return -1;
    }
    if (header.qdCount != 1)
    {
        printf("unsupported query count: %d\n", (int)header.qdCount);
        return -1;
    }

    // handle 1 QUERY
    const char* query = recvBuff + sizeof(dnsHeader);
    const char* nameEnd = query;

    const char* recvBuffEnd = recvBuff + recvSize - 1;
    while(*nameEnd && (nameEnd <= recvBuffEnd)) 
    {
        nameEnd++;
    }

    // Return in case of invalid ending (malformed packet)
    if (nameEnd > recvBuffEnd)
    {
        printf("invalid request\n");
        return -1;
    }
    // skip latest 0 len
    nameEnd++;

    // parse request data
    requestData reqData;
    memcpy(&reqData, nameEnd, sizeof(reqData));
    reqData.qclass = ntohs(reqData.qclass);
    reqData.qtype = ntohs(reqData.qtype);

    if (reqData.qclass != DNS_CLASS_IN)
    {
        printf("unsupported req class: %d\n", reqData.qclass);
        return -1;
    }
    if (reqData.qtype != DNS_TYPE_A)
    {
        printf("unsupported req type: %d\n", reqData.qtype);
        return -1;
    }

    //" Name example: \dgoogle\dcom\0"
    printf("query of len %d: ", (int)(nameEnd - query));

    ////////////////////////////////////////////////////////////////////////////////////
    // /!\ Final part - PRINT domains
    printDomains(query, nameEnd);

    // /!\ Final part - PREPARE response
    // prepare response header
    header.flags = htons(0x8000); // Response
    //header.f.qr = DNS_QR_RESPONSE;

    header.qdCount = 0;
    header.anCount = htons(1); // 1 answer
    header.arCount = 0;
    header.nsCount = 0;

    const char* startPtr = sendBuff;
    char* outPtr = sendBuff;
    memcpy(outPtr, &header, sizeof(header));
    outPtr += sizeof(header);

    // prepare response body -- put answer
    // name
    memcpy(outPtr, query, nameEnd - query);
    outPtr += nameEnd - query;
    // initial request data
    reqData.qclass = htons(reqData.qclass);
    reqData.qtype = htons(reqData.qtype);
    memcpy(outPtr, &reqData, sizeof(reqData));
    outPtr += sizeof(reqData);

    // aData response
    responseAData aData;
    aData.ttl = 0; // 0 seconds TTL
    aData.length = htons(4); // addr size (IPv4, 4 bytes)
    aData.address = htonl(0x01010101); // 1.1.1.1

    memcpy(outPtr, &aData, sizeof(aData));
    outPtr += sizeof(aData);

    return outPtr - startPtr;
}

static int prepareSocket(int argc, char **argv) 
{
    // prepare port
    int port = DEFAULT_PORT;
    if (argc > 1) 
    {
        port = atoi(argv[1]);
    }

    // create UDP socket
    int socFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socFd == -1)
    {
        handleError("socket");
    }

    // prepare address
    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind socket to address
    if (bind(socFd, (struct sockaddr*)&servAddr, sizeof(servAddr) ) == -1)
    {
        handleError("bind");
    }

    return socFd;
}

int main(int argc, char **argv)
{
    if (argc > 1 && ( !strcmp(argv[1], "help") || !strcmp(argv[1], "--help") || !strcmp(argv[1], "-help") ))
    {
        help();
        return -1;
    }

    int servFd = prepareSocket(argc, argv);

    char recvBuff[BUFFER_SIZE];
    char sendBuff[BUFFER_SIZE];

    while(1)
    {
        printf("Waiting for data...\n");

        struct sockaddr_in cliAddr;
        socklen_t slen = sizeof(cliAddr);

        // try to receive some data, this is a blocking call
        int recvSize = recvfrom(servFd, recvBuff, sizeof(recvBuff), 0, (struct sockaddr *) &cliAddr, &slen);

        if (recvSize == -1)
        {
            //TODO: check switch(errno)
            handleError("recvfrom()");
        }

        // process Request & prepare Answer
        const int responseSize = processRequestAndPrepareResponse(recvBuff, recvSize, sendBuff, sizeof(sendBuff));
        if (responseSize > 0)
        {
            // send response to client
            if (sendto(servFd, sendBuff, responseSize, 0, (struct sockaddr*) &cliAddr, slen) == -1)
            {
                handleMinorError("sendto failed");
            }
        }
    }

    return EXIT_SUCCESS;
}
