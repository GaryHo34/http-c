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

typedef struct {
    char *method,   // GET, POST, PUT, DELETE
        *protocol,  // HTTP/1.1
        *route,     // /index.html
        *host,      // localhost:8080
        *msg;       // OK, Not Found, Internal Server Error
    int code;       // 200, 404, 500
} header;

typedef struct {
    header *header;
    char *body;
} request;

typedef struct {
    header *header;
    char *body;
} response;

/**
 * @brief generate request
 * @return request
 */
request *create_request();

/**
 * @brief generate response
 * @return response
 */
response *create_response();

/**
 * @brief parse request
 * @param buf request string
 * @return request
 */
request *parse_request(char *buf);

/**
 * @brief get full response
 * @param res response
 * @param code status code
 * @param msg status message
 * @param body response body
 */
void generate_response(response *res, int code, char *msg, char *body);

/**
 * @brief convert response to string
 * @param res response string
 * @param response
 */
void response_string(char *res_str, response *res);

/**
 * @brief free request
 * @param request
 */
void free_request(request *request);

/**
 * @brief free response
 * @param response
 */
void free_response(response *response);

#endif  // REQUE_H
