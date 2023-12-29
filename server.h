#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>

#include "request.h"

#define BACKLOG 100
#define BUF_SIZE 65535
#define DEFAULT_PORT "3490"
#define DEFAULT_HOST "localhost"

/**
 * @brief initialize server
 * @param sockfd socket file descriptor
 * @param host hostname
 * @param port port number(0 - 65535)
 * @return socket file descriptor
 */
void init_server(int *sockfd, char *host, char *port);

/**
 * @brief run server
 * @param sockfd socket file descriptor
 * @param controller controller function
 * @param thread_num number of threads
 */
void server_run(int sockfd, void (*controller)(request *, response *), int thread_num);

#endif  // SERVER_H