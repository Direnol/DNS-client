
#include "../inc/dns.h"


int main(int argc, char **argv)
{
    char **list = getipbyname("vk.com");
    for (int i = 0; list[i] != NULL; ++i) {
        puts(list[i]);
        free(list[i]);
    }
    free(list);
    return EXIT_SUCCESS;
}
