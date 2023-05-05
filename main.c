#include <stdio.h>

#include "log.h"
#include "request.h"
#include "server.h"

void custom_controller(request_t *req, response_t *res) {
    GET(req, "/") {
        printf("This is a %s request from %s\n",
               req->header->method, req->header->route);
        generate_response(res, 200, "OK", "<h1> Hello World! </h1>");
    }

    POST(req, "/hello") {
        printf("This is a %s request from %s\n",
               req->header->method, req->header->route);
        generate_response(res, 200, "OK", "Hello World!");
    }
}

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

    server_run(sockfd, custom_controller);

    return 0;
}