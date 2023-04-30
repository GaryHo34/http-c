#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#ifndef HTTPC_H
#define HTTPC_H

#define DEFAULT_PORT "3490"
#define DEFAULT_HOST "localhost"

#define ERROR_MSG(fmt, ...)                \
    char _buf[256];                        \
    strerror_r(errno, _buf, sizeof(_buf)); \
    fprintf(stderr, fmt, ##__VA_ARGS__, _buf)

/**
 * param: host, port
 * return socket file descriptor
 */
int init_serv(char *host, char *port);

#endif  // HTTPC_H