#ifndef REQUE_H
#define REQUE_H

#define HTTP_PROTOCOL "HTTP/1.1"

#define DELEMITER " \t\r\n"
#define KEY_VALUE_DELEMITER ":" DELEMITER

#define ROUTE(req, url, met)                 \
    if (strncmp(req->header->route, url, strlen(url)) == 0 && \
        strncmp(req->header->method, met, strlen(met)) == 0) 

#define GET(req, url) ROUTE(req, url, "GET")
#define POST(req, url) ROUTE(req, url, "POST")
#define PUT(req, url) ROUTE(req, url, "PUT")
#define DELETE(req, url) ROUTE(req, url, "DELETE")

// generate the response string
#define RESPONSE(code, body)

typedef struct header {
    char *method,   // GET, POST, PUT, DELETE
        *protocol,  // HTTP/1.1
        *route,     // /index.html
        *host;      // localhost:8080
} header_t;

typedef struct request {
    struct header *header;
    char *body;
} request_t;

/**
 * @brief generate request
 * @return request
 */
request_t *generate_request();

/**
 * @brief parse request
 * @return request
 */
request_t *parse_request(char *buf);

char *generate_response(int code, char *body);

/**
 * @brief free request
 * @param request
 */
void free_request(request_t *request);

#endif  // REQUE_H
