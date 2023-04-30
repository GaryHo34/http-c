#include "server.h"

#include "log.h"

int init_serv(char *host, char *port) {
    // port numbers range from 0 to 65536
    int portnum;
    sscanf(port, "%d", &portnum);
    if (portnum < 0 || portnum > 65536) {
        PANIC("[server] %s", "invalid port number");
    }

    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv, option = 1;  // option_vale of SO_REUSEADDR

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(host, port, &hints, &servinfo)) != 0) {
        PANIC("[server] %s: %s", "getaddrinfo", gai_strerror(rv));
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            ERROR_MSG("[server] %s", "failed to create socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option,
                       sizeof(int)) == -1) {
            PANIC("[server] %s", "failed to setsockopt");
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            ERROR_MSG("[server] %s", "failed to bind");
            continue;
        }

        break;
    }

    // if p is NULL, means we didn't get bound, note the continue in the loop
    if (p == NULL) {
        PANIC("[server] %s", "no address succeeded");
    }

    freeaddrinfo(servinfo);  // no need for res more

    if (listen(sockfd, BACKLOG) == -1) {
        PANIC("[server] %s", "failed to listen");
    }

    LOG_MSG("[server] %s %s:%s", "listening on", host, port);

    return sockfd;
}
