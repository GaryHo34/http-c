#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#ifndef SERVER_H
#define SERVER_H

#define BACKLOG 100
#define DEFAULT_PORT "3490"
#define DEFAULT_HOST "localhost"

/**
 * @brief initialize server
 * @param host host name
 * @param port port number(0 - 65535)
 * @return socket file descriptor
 */
int init_serv(char *host, char *port);

#endif  // SERVER_H