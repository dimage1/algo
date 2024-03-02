#include "precomp.h"

#define handleError(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

enum {
    MAX_EVENTS   = 10,
    BUFF_SIZE    = 64,
    DEFAULT_PORT = 8888
};

static std::set<int> clients;

static int prepareSocket(int argc, char **argv) {
    int                 sfd, cfd;
    socklen_t           peer_addr_size;
    struct sockaddr_un  my_addr, peer_addr;

    // create socket
    int soc = socket(AF_INET, SOCK_STREAM, 0);
    if (soc == -1) {
        handleError("socket");
    }

    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(argc > 1 ? atoi(argv[1]) : DEFAULT_PORT);

    // bind address
    if (bind(soc, (struct sockaddr *) &servAddr, sizeof(servAddr)) == -1) {
        handleError("bind");
    }

    // start listen to clients
    if (listen(soc, SOMAXCONN) == -1)
        handleError("listen");

    return soc;
}


static int prepareEpoll(int socFd) {
    int epollfd = epoll_create1(0);
    if (epollfd == -1) {
        handleError("epoll_create1");
    }

    struct epoll_event ev;

    ev.events = EPOLLIN;
    ev.data.fd = socFd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, socFd, &ev) == -1) {
        handleError("epoll_ctl: listen_sock");
    }

    return epollfd;
}


void onClientConnected(int clientFd) {
    clients.insert(clientFd);
}

void onClientDisconnected(int clientFd) {
    clients.erase(clientFd);
}

void relayData (const char* buff, size_t size, int excludeClient) {
    for (int fd: clients) {
        if (fd != excludeClient) {
            write(fd, buff, size);
        }
    }
}

static void processConnection(int listenSoc, int listenEpoll, int fd) {
    struct sockaddr addr;
    socklen_t addrlen;

    int connSock = accept4(listenSoc, (struct sockaddr *) &addr, &addrlen, SOCK_NONBLOCK);
    if (connSock == -1) {
        handleError("accept");
    }

    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    ev.data.fd = connSock;
    if (epoll_ctl(listenEpoll, EPOLL_CTL_ADD, connSock, &ev) == -1) {
        handleError("epoll_ctl: conn_sock");
    }
    onClientConnected(connSock);
}

static void processDisconnection(int fd, int listenEpoll) {
    struct epoll_event ev;

    ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    ev.data.fd = fd;

    if (epoll_ctl(listenEpoll, EPOLL_CTL_DEL, fd, &ev) == -1) {
        handleError("epoll_ctl: EPOLL_CTL_DEL");
    }
    shutdown(fd, SHUT_RDWR);
    close(fd);

    onClientDisconnected(fd);
}

static void processIncomingData(int fd) {
    char buff[BUFF_SIZE];
    while (1) {
        size_t size = read(fd, buff, sizeof(buff));

        if (size > 0 && size <= sizeof(buff)) {
            printf("Got %zu bytes from %d\n", size, fd);
            relayData(buff, size, fd);
        }
        else {
            break;
        }
    }
}

int main(int argc, char **argv)
{
    const int listenSoc = prepareSocket(argc, argv);
    const int listenEpoll = prepareEpoll(listenSoc);

    struct epoll_event events[MAX_EVENTS];

    // Working loop, wait for epoll events
    for (;;) {
        const int nfds = epoll_wait(listenEpoll, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            handleError("epoll_wait");
        }

        // process events
        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == listenSoc) {
                processConnection(listenSoc, listenEpoll, events[n].data.fd);
            }
            else if (events[n].events & EPOLLRDHUP) {
                processDisconnection(events[n].data.fd, listenEpoll);
            }
            else {
                processIncomingData(events[n].data.fd);
            }
        }
    }

    return EXIT_SUCCESS;
}