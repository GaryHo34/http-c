#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    struct addrinfo hints, *res, *p;
    int status;

    // defined in netinet/in.h
    char ipstr[INET6_ADDRSTRLEN];

    if (argc != 2)
    {
        fprintf(stderr, "Error: showip hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0)
    {
        fprintf(stderr, "[ERROR]: %s\n", gai_strerror(status));
        return 2;
    }

    fprintf(stdout, "[LOG]  : IP addresses for %s:\n\n", argv[1]);

    for (p = res; p != NULL; p = p->ai_next)
    {
        void *addr;
        char *ipver;
        if (p->ai_addr->sa_family == AF_INET)
        {
            ipver = "IPv4";
        }
        else
        {
            ipver = "IPv6";
        }
        inet_ntop(p->ai_family, &(p->ai_addr), ipstr, sizeof ipstr);

        fprintf(stdout, " %s: %s\n", ipver, ipstr);
    }

    freeaddrinfo(res); // free the linked list

    return 0;
}