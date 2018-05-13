#include "../inc/dns.h"

char dip[] = "192.168.1.1";


struct iovec *pack_dns(dns_t *dns, query_t *query, size_t n)
{
    struct iovec *iov = calloc(n + 1, sizeof(*iov));
    if (!iov) return NULL;

    iov[0].iov_base = memdup(dns, sizeof(*dns));
    iov[0].iov_len = sizeof(*dns);

    size_t cur_size = 0;
    char buf[UINT16_MAX];
    size_t uint2 = sizeof(uint16_t) * 2;

    for (int i = 0; i < (n); ++i) {
        ChangetoDnsNameFormat(buf, query[i].name);
        cur_size = strlen(buf) + 1;
        memcpy(buf + cur_size, &query[i].type, uint2);
        iov[i + 1].iov_len = cur_size + uint2;
        iov[i + 1].iov_base = memdup(buf, cur_size + uint2);
    }

    return iov;
}

char *getipbyname(char *name)
{
    srand((unsigned int) time(NULL));
    int sock = 0;
    ERR(init_sock(&sock), "Init socket");
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(53);
    addr.sin_addr.s_addr = inet_addr(dip);

    uint16_t count_query = 1;
    dns_t dns;
    memset(&dns, 0, sizeof(dns));
    dns.id = htons((uint16_t) rand());
    dns.flags = htons(0x0100);
    dns.quests = htons(count_query);

    query_t query;
    query.name = name;
    query.class = query.type = htons(0x0001);

    struct iovec *iov = pack_dns(&dns, &query, count_query);
    ERR(send_pack(sock, iov, count_query + 1, &addr), "Send pack");
    recv_pack();

    for (int i = 0; i < count_query + 1; ++i) {
        free(iov[i].iov_base);
    }
    free(iov);
    return NULL;
}

void ChangetoDnsNameFormat(char *dns, char *_host)
{
    int lock = 0;
    char *host = malloc(strlen(_host) + 2);
    strcpy(host, _host);
    strcat(host, ".");

    for (int i = 0; i < strlen(host); i++) {
        if (host[i] == '.') {
            *dns++ = (unsigned char) (i - lock);
            for (; lock < i; lock++) {
                *dns++ = host[lock];
            }
            lock++;
        }
    }
    *dns++ = '\0';
    free(host);
}

void get_dns_servers()
{
    FILE *fp;
    char line[200] , *p;
    if((fp = fopen("/etc/resolv.conf" , "r")) == NULL)
    {
        printf("Failed opening /etc/resolv.conf file \n");
    }

    while(fgets(line , 200 , fp))
    {
        if(line[0] == '#')
        {
            continue;
        }
        if(strncmp(line , "nameserver" , 10) == 0)
        {
            p = strtok(line , " ");
            p = strtok(NULL , " ");

            //p now is the dns ip
        }
    }
}
