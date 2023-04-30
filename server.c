#include "server.h"

#include "log.h"

static inline void get_sockaddr(char *addrstr, struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {  // IPv4
        char ipv4[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr),
                  ipv4, sizeof ipv4);
        strncpy(addrstr, ipv4, INET_ADDRSTRLEN);
    } else {  // IPv6
        char ipv6[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr),
                  ipv6, sizeof ipv6);
        strncpy(addrstr, ipv6, INET6_ADDRSTRLEN);
    }
}

void init_server(int *sockfd, char *hostname, char *port) {
    // port numbers range from 0 to 65536
    int portnum;
    sscanf(port, "%d", &portnum);
    if (portnum < 0 || portnum > 65536) {
        PANIC("[server] %s", "invalid port number");
    }

    signal(SIGCHLD, SIG_IGN);  // avoid zombie process

    struct addrinfo hints, *servinfo, *p;
    int rv, option = 1;  // option_vale of SO_REUSEADDR

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(hostname, port, &hints, &servinfo)) != 0) {
        PANIC("[server] %s: %s", "getaddrinfo", gai_strerror(rv));
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((*sockfd = socket(p->ai_family, p->ai_socktype,
                              p->ai_protocol)) == -1) {
            ERROR_MSG("[server] %s", "failed to create socket");
            continue;
        }

        if (setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &option,
                       sizeof(int)) == -1) {
            close(*sockfd);
            PANIC("[server] %s", "failed to setsockopt");
        }

        if (bind(*sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(*sockfd);
            ERROR_MSG("[server] %s", "failed to bind");
            continue;
        }

        break;
    }

    // if p is NULL, means we didn't get bound, note the continue in the loop
    if (p == NULL) {
        close(*sockfd);
        PANIC("[server] %s", "no address succeeded");
    }

    freeaddrinfo(servinfo);  // no need for res more

    if (listen(*sockfd, BACKLOG) == -1) {
        close(*sockfd);
        PANIC("[server] %s", "failed to listen");
    }

    LOG_MSG("[server] %s %s:%s", "listening on", hostname, port);

    return;
}

void server_run(int sockfd) {
    int clientfd;
    struct sockaddr_storage inaddr;  // connector's address information
    char addrstr[BUF_SIZE];
    socklen_t sin_size;

    LOG_MSG("[server] %s", "waiting for connection...");

    while (1) {  // main accept() loop
        sin_size = sizeof inaddr;
        clientfd = accept(sockfd, (struct sockaddr *)&inaddr, &sin_size);

        if (clientfd == -1) {
            ERROR_MSG("[server] %s", "failed to accept");
            continue;
        }

        get_sockaddr(addrstr, (struct sockaddr *)&inaddr);

        LOG_MSG("[server] got connection from %s", addrstr);

        if (!fork()) {      // this is the child process
            close(sockfd);  // child doesn't need the listener
            char *buf = malloc(BUF_SIZE);
            int rcvd = recv(clientfd, buf, BUF_SIZE, 0);
            if (rcvd == -1)
                perror("send");
            buf[rcvd] = '\0';
            char *method,   // "GET" or "POST"
                *uri,       // "/index.html" things before '?'
                *protocol;  // "HTTP/1.1"

            // method = strtok(buf, " \t\r\n");
            // uri = strtok(NULL, " \t");
            // protocol = strtok(NULL, " \t\r\n");
            fprintf(stdout, "REQ: %s\n", buf);
            // fprintf(stdout, "URL: %s\n", uri);
            // fprintf(stdout, "PROT: %s\n", protocol);
            char res[] = "HTTP/1.1 200 OK \t\r\n";

            send(clientfd, res, strlen(res), 0);
            close(clientfd);
            exit(0);
        }
        close(clientfd);  // parent doesn't need this
    }
}
