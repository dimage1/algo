#include <bits/stdc++.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <sstream>

////////////////////////////////////////////////////////////////////////
struct dns_header_t {
     uint16_t     id;
     uint16_t     flags;    // 0x1000 for request
     uint16_t     qdcount;  // questions count
     uint16_t     ancount;  // answer counts
     uint16_t     nscount;  // authority records
     uint16_t     arcount;  // additional records
};

struct dns_question
{
    u_int16_t qtype;
    u_int16_t qclass;
};

enum {
    DNS_T_A = 1,
    DNS_T_AAAA = 28,
};

////////////////////////////////////////////////////////////////////////
// ./dnsClient google.com A
int main(int argc, char **argv) {
    auto domain = std::string{".ya.ru"};
    if (argc > 1) {
        domain.assign(argv[1]);
        if (domain[0] != '.') {
            domain = '.' + domain;
        }
    }
    int reqType = DNS_T_A;
    if (argc > 2) {
        std::string type(argv[2]);
        reqType = type == "AAAA" ? DNS_T_AAAA : DNS_T_A;
    }

    int sockfd; 
    const char *hello = "Hello from client"; 
    struct sockaddr_in     servaddr;
   
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
   
    memset(&servaddr, 0, sizeof(servaddr)); 
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(53);  // UDP DNS
    //inet_aton ("201.6.0.103", &servaddr.sin_addr);
    inet_aton ("8.8.8.8", &servaddr.sin_addr);

    dns_header_t header;
    memset(&header, 0, sizeof(header));
    header.id = htons(time(NULL) & 0xFFFF);
    header.flags = 0x0100;
    header.qdcount = htons(1);

    char bufTx[64];
    memcpy (bufTx, &header, sizeof(header));
    size_t len = sizeof(header);

    // prepare domain part
    memcpy (bufTx + len, domain.c_str(), domain.size() + 1);

    std::istringstream f(domain.c_str() + 1);
    std::string s;
    size_t nextLabel = len;
    while (getline(f, s, '.')) {
        bufTx[nextLabel] = s.size();
        nextLabel += s.size() + 1;
    }
    len += domain.size() + 1;

    dns_question q;
    q.qtype = htons(reqType);
    q.qclass = htons(1);

    memcpy (bufTx + len, &q, sizeof(q));
    len += sizeof(q);


    sendto(sockfd, bufTx, len, MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
    std::cout<<"1st message sent." <<std::endl; 

    char bufRx[512];
    socklen_t sockLen = sizeof(servaddr);
    //int n = recvfrom(sockfd, (char *)bufRx, sizeof(bufRx), MSG_WAITALL, (struct sockaddr*)&servaddr, &sockLen); 
    int n = recv(sockfd, bufRx, sizeof(bufRx), 0);
    printf("Received : %d bytes: %s\n", n, bufRx); 

    close(sockfd); 
    return 0; 
}