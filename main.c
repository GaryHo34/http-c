
#include <unistd.h>

#include "log.h"
#include "request.h"
#include "server.h"
#include "threadp.h"
void custom_controller(request *req, response *res) {
    // simulate a long request
    usleep(50000);

    GET(req, "/") {
        printf("This is a %s request to route %s\n",
               req->header->method, req->header->route);
        generate_response(res, 200, "OK", "<h1> Hello World! </h1>");
        return;
    }

    POST(req, "/hello") {
        printf("This is a %s request to route %s\n",
               req->header->method, req->header->route);
        generate_response(res, 200, "OK", "Hello World!");
        return;
    }

    generate_response(res, 404, "Not Found", "");
}

int main(int argc, char *argv[]) {
    // get host and port from command line
    char *hostname = DEFAULT_HOST;
    char *port = DEFAULT_PORT;
    int thread_num = DEFAULT_THREAD_NUM;
    int sockfd;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-hostname") == 0) {
            hostname = argv[++i];
        } else if (strcmp(argv[i], "-port") == 0) {
            port = argv[++i];
        } else if (strcmp(argv[i], "-thread") == 0) {
            int num = atoi(argv[++i]);
            if (num > 0 && num <= 32) {
                thread_num = num;
            }
        }
    }

    init_server(&sockfd, hostname, port);

    server_run(sockfd, custom_controller, thread_num);

    return 0;
}