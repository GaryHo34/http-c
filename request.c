#include "request.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

request_t* generate_request_t() {
    request_t* req = malloc(sizeof(request_t));
    req->body = NULL;
    req->header = malloc(sizeof(header_t));
    return req;
}

response_t* generate_response_t() {
    response_t* res = malloc(sizeof(response_t));
    res->body = NULL;
    res->header = malloc(sizeof(header_t));
    return res;
}

request_t* parse_request(char* buf) {
    request_t* req = generate_request_t();

    req->header->method = strtok_r(buf, DELEMITER, &buf);
    req->header->route = strtok_r(buf, DELEMITER, &buf);
    req->header->protocol = strtok_r(buf, DELEMITER, &buf);
    strtok_r(buf, KEY_VALUE_DELEMITER, &buf);
    req->header->host = strtok_r(buf, DELEMITER, &buf);
    req->body = buf;
    return req;
}

void generate_response(response_t* res, int code, char* msg, char* body) {
    res->header->code = code;
    res->header->msg = msg;
    res->body = body;
}

void response_to_string(char* res_str, response_t* res) {
    sprintf(res_str, "%s %d %s\r\n\r\n%s",
            HTTP_PROTOCOL,
            res->header->code,
            res->header->msg,
            res->body);
}

void free_request_t(request_t* request) {
    free(request->header);
    free(request);
}

void free_response_t(response_t* response) {
    free(response->header);
    free(response);
}