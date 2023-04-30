#include "server.h"

int init_serv(char *host, char *port) {
    // port numbers range from 0 to 65536
    int portnum;
    sscanf(port, "%d", &portnum);
    if (portnum < 0 || portnum > 65536) {
        ERROR_MSG("[server] %s\n", "invalid port number");
        exit(EXIT_FAILURE);
    }

    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv, option = 1;  // option_vale of SO_REUSEADDR

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(host, port, &hints, &servinfo)) != 0) {
        ERROR_MSG("[server] %s: %s\n", "getaddrinfo", gai_strerror(rv));
        exit(EXIT_FAILURE);
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            ERROR_MSG("[server] %s\n", "failed to create socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option,
                       sizeof(int)) == -1) {
            ERROR_MSG("[server] %s\n", "failed to setsockopt");
            exit(EXIT_FAILURE);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            ERROR_MSG("[server] %s\n", "failed to bind");
            continue;
        }

        break;
    }

    // if p is NULL, means we didn't get bound, note the continue in the loop
    if (p == NULL) {
        ERROR_MSG("[server] %s\n", "failed to bind");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(servinfo);  // no need for res more

    if (listen(sockfd, BACKLOG) == -1) {
        ERROR_MSG("[server] %s: %s\n", "failed to listen");
        exit(EXIT_FAILURE);
    }

    
    return sockfd;
}
