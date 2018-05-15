#include "../inc/socket.h"

int init_sock(int *fd)
{
    *fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (*fd < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int send_pack(int fd, struct iovec *iov, size_t n, struct sockaddr_in *addr)
{
    struct msghdr msg;
    memset(&msg, 0, sizeof(msg));
    msg.msg_iov = iov;
    msg.msg_iovlen = n;

    msg.msg_name = addr;
    msg.msg_namelen = sizeof(*addr);

    ssize_t ret = sendmsg(fd, &msg, 0);
    if (ret <= 0) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

void *memdup(void *mem, size_t n)
{
    void *new_mem = malloc(n);
    memcpy(new_mem, mem, n);
    return new_mem;
}

int recv_pack(int fd, char *pack, struct sockaddr *sock, size_t *nbytes)
{
    socklen_t socklen = sizeof(*sock);
    ssize_t count = recvfrom(fd, pack, *nbytes, 0, sock, &socklen);
    if (count < 0) {
        return EXIT_FAILURE;
    }
    *nbytes = (size_t) count;


    return EXIT_SUCCESS;
}

