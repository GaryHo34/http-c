#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "server.h"
#include "log.h"

#define BUF_SIZE 65535

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main(int argc, char *argv[]) {
    // get host and port from command line
    char *host = DEFAULT_HOST;
    char *port = DEFAULT_PORT;

    if (argc == 2) {
        host = argv[1];
    } else if (argc == 3) {
        host = argv[1];
        port = argv[2];
    }

    int sockfd = init_serv(host, port);

    int new_fd;                          // listen on sock_fd, new connection on new_fd
    struct sockaddr_storage their_addr;  // connector's address information
    char s[INET6_ADDRSTRLEN];
    socklen_t sin_size;

    printf("server: waiting for connections...\n");

    while (1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            ERROR_MSG("[server] %s", "failed to accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *)&their_addr),
                  s, sizeof s);
        printf("server: got connection from %s\n", s);

        if (!fork()) {      // this is the child process
            close(sockfd);  // child doesn't need the listener
            char *buf = malloc(BUF_SIZE);
            int rcvd = recv(new_fd, buf, BUF_SIZE, 0);
            if (rcvd == -1)
                perror("send");
            buf[rcvd] = '\0';
            char *method,   // "GET" or "POST"
                *uri,       // "/index.html" things before '?'
                *protocol;  // "HTTP/1.1"

            method = strtok(buf, " \t\r\n");
            uri = strtok(NULL, " \t");
            protocol = strtok(NULL, " \t\r\n");
            fprintf(stdout, "METHOD: %s\n", method);
            fprintf(stdout, "URL: %s\n", uri);
            fprintf(stdout, "PROT: %s\n", protocol);
            char res[] = "HTTP/1.1 200 OK \t\r\n";

            send(new_fd, res, strlen(res), 0);
            close(new_fd);
            exit(0);
        }
        close(new_fd);  // parent doesn't need this
    }

    return 0;
}