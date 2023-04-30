#include "log.h"
#include "server.h"

int main(int argc, char *argv[]) {
    // get host and port from command line
    char *hostname = DEFAULT_HOST;
    char *port = DEFAULT_PORT;

    int sockfd;

    if (argc == 2) {
        hostname = argv[1];
    } else if (argc == 3) {
        hostname = argv[1];
        port = argv[2];
    }
    init_server(&sockfd, hostname, port);

    server_run(sockfd);

    return 0;
}