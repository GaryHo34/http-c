#include <arpa/inet.h>
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
 * @brief cast sockaddr to sockaddr_in or sockaddr_in6
 * @param addrstr address string
 * @param sa sockaddr
 * @return IPv4 or IPv6 address
 */
static inline void get_sockaddr(char *addrstr, struct sockaddr *sa);

/**
 * @brief run server
 * @param sockfd socket file descriptor
 */
void server_run(int sockfdÏ);

#endif  // SERVER_H