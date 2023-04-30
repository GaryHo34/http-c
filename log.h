#ifndef LOG_H
#define LOG_H

#define LOG_MSG(fmt, ...) fprintf(stdout, fmt "\n", ##__VA_ARGS__)

#define ERROR_MSG(fmt, ...)                  \
    char _buf[256];                          \
    strerror_r(errno, _buf, sizeof(_buf));   \
    if (!errno)                              \
        fprintf(stderr, fmt "\n", ##__VA_ARGS__); \
    else                                     \
        fprintf(stderr, fmt ": %s\n", ##__VA_ARGS__, _buf)

#define PANIC(fmt, ...)            \
    ERROR_MSG(fmt, ##__VA_ARGS__); \
    exit(EXIT_FAILURE)

#endif