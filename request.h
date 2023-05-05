#ifndef REQUE_H
#define REQUE_H

#define HTTP_PROTOCOL "HTTP/1.1"

#define DELEMITER " \r\n"
#define KEY_VALUE_DELEMITER ":" DELEMITER

#define ROUTE(req, url, met)                                  \
    if (strncmp(req->header->route, url, strlen(url)) == 0 && \
        strncmp(req->header->method, met, strlen(met)) == 0)

#define GET(req, url) ROUTE(req, url, "GET")
#define POST(req, url) ROUTE(req, url, "POST")
#define PUT(req, url) ROUTE(req, url, "PUT")
#define DELETE(req, url) ROUTE(req, url, "DELETE")

typedef struct header {
    char *method,   // GET, POST, PUT, DELETE
        *protocol,  // HTTP/1.1
        *route,     // /index.html
        *host,      // localhost:8080
        *msg;       // OK, Not Found, Internal Server Error
    int code;       // 200, 404, 500
} header_t;

typedef struct request {
    struct header *header;
    char *body;
} request_t;

typedef struct response {
    struct header *header;
    char *body;
} response_t;

/**
 * @brief generate request
 * @return request_t
 */
request_t *generate_request_t();

/**
 * @brief generate response
 * @return response_t
 */
response_t *generate_response_t();

/**
 * @brief parse request
 * @param buf request string
 * @return request
 */
request_t *parse_request(char *buf);

/**
 * @brief get full response
 * @param res response_t
 * @param code status code
 * @param msg status message
 * @param body response body
 */
void generate_response(response_t *res, int code, char *msg, char *body);

/**
 * @brief convert response to string
 * @param res response string
 * @param response_t
 */
void response_to_string(char *res_str, response_t *res);

/**
 * @brief free request_t
 * @param request
 */
void free_request_t(request_t *request);

/**
 * @brief free response_t
 * @param response_t
 */
void free_response_t(response_t *response);

#endif  // REQUE_H
