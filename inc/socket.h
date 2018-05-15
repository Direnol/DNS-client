#ifndef DNS_CLIENT_SOCKET_H
#define DNS_CLIENT_SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>

#include <sys/socket.h>
#include <linux/ioctl.h>
#include <linux/if_ether.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <arpa/nameser.h>


int init_sock(int *fd);

/* 0 - success, 1 - timeout */
int recv_pack(int fd, char *pack, struct sockaddr *sock, size_t *nbytes);

int send_pack(int fd, struct iovec *iov, size_t n, struct sockaddr_in * addr);

void *memdup(void *mem, size_t n);

#endif //DNS_CLIENT_SOCKET_H
