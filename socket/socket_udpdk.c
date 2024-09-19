#include <signal.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <netinet/tcp.h>

// https://github.com/leoll2/UDPDK -- used outdated DPDK api, requires extra process
// afterall, not working as expected
#include <udpdk_api.h>

static void signal_handler(int signum)
{
    printf("Caught signal %d in pingpong main process\n", signum);
    udpdk_interrupt(signum);
}

//**********************************************************************************************************************
int e_init(void)
{
    char **args = {"app", "-c", "dpdk.ini"};
    udpdk_init(3, args);

    // Register signals for shutdown
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    return 0;
}

//**********************************************************************************************************************
void e_deinit(void)
{
    udpdk_interrupt(0);
    udpdk_cleanup();
}

//**********************************************************************************************************************
int e_socket_bind(ESocket& socket, const EAddress* address)
{
    struct sockaddr_in sin;

    if (address != nullptr)
    {
        e_address_to_sockaddr(*address, sin);
    }
    else
    {
        e_address_reset_sockaddr(sin);
    }

    if (udpdk_bind(socket.handle, (const struct sockaddr *)&sin, sizeof(struct sockaddr_in)) == -1)
    {
        return -1;
    }

    return 0;
}

//**********************************************************************************************************************
ESocket e_socket_create(ESocketType type)
{
    result.handle = udpdk_socket(AF_INET, type == E_SOCKET_TYPE_DATAGRAM ? SOCK_DGRAM : SOCK_STREAM, 0);
    return result;
}

//**********************************************************************************************************************
int e_socket_set_option(ESocket& socket, ESocketOption option, int value)
{
    int result = -1;
    switch (option)
    {
    case E_SOCKOPT_NONBLOCK:
    {
        // not implemented in udpdk
        return 0;
    }
    break;

    case E_SOCKOPT_BROADCAST:
        result = udpdk_setsockopt(socket.handle, SOL_SOCKET, SO_BROADCAST, (char *)& value, sizeof(int));
        break;

    case E_SOCKOPT_REUSEADDR:
        result = udpdk_setsockopt(socket.handle, SOL_SOCKET, SO_REUSEADDR, (char *)& value, sizeof(int));
        break;

    case E_SOCKOPT_RCVBUF:
        result = udpdk_setsockopt(socket.handle, SOL_SOCKET, SO_RCVBUF, (char *)& value, sizeof(int));
        break;

    case E_SOCKOPT_SNDBUF:
        result = udpdk_setsockopt(socket.handle, SOL_SOCKET, SO_SNDBUF, (char *)& value, sizeof(int));
        break;

    case E_SOCKOPT_RCVTIMEO:
    {
        struct timeval timeVal;
        timeVal.tv_sec = value / 1000;
        timeVal.tv_usec = (value % 1000) * 1000;
        result = udpdk_setsockopt(socket.handle, SOL_SOCKET, SO_RCVTIMEO, (char *)& timeVal, sizeof(struct timeval));
    }
    break;

    case E_SOCKOPT_SNDTIMEO:
    {
        struct timeval timeVal;
        timeVal.tv_sec = value / 1000;
        timeVal.tv_usec = (value % 1000) * 1000;
        result = udpdk_setsockopt(socket.handle, SOL_SOCKET, SO_SNDTIMEO, (char *)& timeVal, sizeof(struct timeval));
    }
    break;

    case E_SOCKOPT_NODELAY:
        result = udpdk_setsockopt(socket.handle, IPPROTO_TCP, TCP_NODELAY, (char *)& value, sizeof(int));
        break;
    default:
        break;
    }

    if (result == -1)
    {
        return -1;
    }

    return 0;
}

void e_socket_destroy(ESocket& socket)
{
    if (socket.handle != E_SOCKET_INVALID_HANDLE)
    {
        udpdk_close(socket.handle);
        socket.handle = E_SOCKET_INVALID_HANDLE;
    }
}

int e_socket_send(ESocket& socket, const EAddress* address, const EBuffer * buffers, size_t bufferCount)
{
    if (bufferCount == 0)
    {
        return 0;
    }
    return udpdk_sendto(socket.handle, buffers[0].data, buffers[0].dataLength, MSG_DONTWAIT | MSG_NOSIGNAL,
        (struct sockaddr*)&sin, sizeof(struct sockaddr));
}

int e_socket_receive(ESocket& socket, EAddress* address, EBuffer* buffer)
{
    struct sockaddr_in sin;
    if (address != nullptr)
    {
        e_address_reset_sockaddr(sin);

    }

    socklen_t soclen = sizeof(struct sockaddr);
    return udpdk_recvfrom(socket.handle, buffer->data, buffer->dataLength, MSG_NOSIGNAL, (struct sockaddr*)&sin, &soclen);
}

