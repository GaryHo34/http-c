#include "request.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

request* create_request() {
    request* req = malloc(sizeof(request));
    req->body = NULL;
    req->header = malloc(sizeof(header));
    return req;
}

response* create_response() {
    response* res = malloc(sizeof(response));
    res->body = NULL;
    res->header = malloc(sizeof(header));
    return res;
}

request* parse_request(char* buf) {
    request* req = create_request();

    req->header->method = strtok_r(buf, DELEMITER, &buf);
    req->header->route = strtok_r(buf, DELEMITER, &buf);
    req->header->protocol = strtok_r(buf, DELEMITER, &buf);
    strtok_r(buf, KEY_VALUE_DELEMITER, &buf);
    req->header->host = strtok_r(buf, DELEMITER, &buf);
    req->body = buf;
    return req;
}

void generate_response(response* res, int code, char* msg, char* body) {
    res->header->code = code;
    res->header->msg = msg;
    res->body = body;
}

void response_string(char* res_str, response* res) {
    sprintf(res_str, "%s %d %s\r\n\r\n%s",
            HTTP_PROTOCOL,
            res->header->code,
            res->header->msg,
            res->body);
}

void free_request(request* request) {
    free(request->header);
    free(request);
}

void free_response(response* response) {
    free(response->header);
    free(response);
}