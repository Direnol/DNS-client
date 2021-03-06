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

char **getipbyname(char *name)
{
    void **ret = NULL;
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

    char *buf = calloc(UINT16_MAX, sizeof(char));
    size_t n = UINT16_MAX;
    if (recv_pack(sock, buf, (struct sockaddr *) &addr, &n)) {
        perror("Fail recv pack");
        ret = NULL;
        goto fail;
    }

    dns_t *ans = (dns_t *) buf;
    count_query = ntohs(ans->quests);
    char *query_start = (buf + sizeof(dns));
    printf("Questions %d ip\n", count_query);

    char *ptr = query_start;
    for (int i = 0; i < count_query; ++i) {
        int size_q = read_query(query_start, &query);
        printf("%s len:[%d] type:[%d] class:[%d]\n", query.name, size_q, query.type, query.class);
        printf("Get %d ip\n", ntohs(ans->answer_rrs));
        ptr += size_q;
    }

    resp_t *resp = (resp_t *) ptr;
    char **list = calloc(ntohs(ans->answer_rrs) + 1, sizeof(*list));
    for (int i = 0; i < ntohs(ans->answer_rrs); ++i) {
      list[i] = strdup(inet_ntoa(resp[i].addr));
    }
    ret = (void **) list;

    fail:
    for (int i = 0; i < count_query + 1; ++i) {
        free(iov[i].iov_base);
    }
    free(iov);
    free(buf);
    return (char **) ret;
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

int read_query(char *buf, query_t *query)
{
    int size = 0;
    for (; buf[size] != '\0'; ++size) {
        if (buf[size] < 'a') buf[size] = '.';
    }
    query->name = buf + 1;
    query->type = ntohs(*(uint16_t *)(buf + size + 1));
    query->class = ntohs(*((uint16_t*) (buf + size + 1 + sizeof(uint16_t))));
    return (int) (strlen(buf) + 2 * sizeof(uint16_t) + 1);
}
