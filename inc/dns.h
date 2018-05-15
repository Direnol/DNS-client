#ifndef DNS_CLIENT_DNS_H
#define DNS_CLIENT_DNS_H

#include "socket.h"
#include <time.h>


#define ERR(x, str) if (x) { perror(str); return NULL; }

enum FLAGS {
    Response = 1,
    // Opcode 2, 4, 8, 16
    Truncated = 32,
    Recursion = 64,
    // Zero = 128
    // Reply code 256, 512, 1024, 2048
};

#define BITSET(num, bit) ((num) | (bit))
#define BITDEL(num, bit) ((num) & (~(bit))

struct _dns {
    uint16_t id;
    uint16_t flags;
    uint16_t quests;

    uint16_t answer_rrs;
    uint16_t auth_rrs;
    uint16_t add_rrs;
};
typedef struct _dns dns_t;

struct _query {

    char *name;
    uint16_t type;
    uint16_t class;
};
typedef struct _query query_t;

#pragma pack(push, 1))
struct _response_dns {
    uint16_t name_ptr;
    uint16_t type;
    uint16_t class;
    uint32_t ttl;
    uint16_t data_len;
    struct in_addr addr;
};
#pragma pack(pop)
typedef struct _response_dns resp_t;

struct iovec * pack_dns(dns_t *dns, query_t *query, size_t n);

// This will convert www.google.com to 3www6google3com
void ChangetoDnsNameFormat(char *dns, char *host);

void get_dns_servers();

int read_query(char *buf, query_t *query);

char ** getipbyname(char *name);

#endif //DNS_CLIENT_DNS_H
