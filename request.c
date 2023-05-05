#include "request.h"

#include <stdlib.h>
#include <string.h>

request_t* generate_request() {
    struct request* req = malloc(sizeof(request_t));
    req->body = NULL;
    req->header = malloc(sizeof(header_t));
    return req;
}

request_t* parse_request(char* buf) {
    request_t* req = generate_request();

    req->header->method = strtok_r(buf, DELEMITER, &buf);
    req->header->route = strtok_r(buf, DELEMITER, &buf);
    req->header->protocol = strtok_r(buf, DELEMITER, &buf);
    strtok_r(buf, KEY_VALUE_DELEMITER, &buf);
    req->header->host = strtok_r(buf, DELEMITER, &buf);

    return req;
}

void free_request(request_t* request) {
    free(request->header);
    free(request);
}