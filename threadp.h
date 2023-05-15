#ifndef THREADP_H
#define THREADP_H

#include <pthread.h>

#include "taskqueue.h"
#define DEFAULT_THREAD_NUM 4

typedef struct thread {
    int id;
    pthread_t pthread;
    struct threadpool *thpool;  // point back to the threadpool
} thread;

typedef struct threadpool {
    thread **threads;  // an array of thread struct pointers
    pthread_mutex_t mutexQueue;
    pthread_cond_t condQueue;
    pthread_mutex_t mutexThreadCount;
    pthread_mutex_t mutexThreadLife;
    pthread_cond_t condThreadCount;
    volatile int thread_alive;
    volatile int num_of_threads;
    volatile int num_of_working_threads;
    taskqueue queue;
} threadpool;

/**
 * @brief Create a new threadpool
 * @param num_of_threads The number of threads to create
 * @return The threadpool created
 */
threadpool *threadpool_init(int num_of_threads);

/**
 * @brief Add a task to the threadpool
 * @param pool The threadpool to add the task to
 * @param func The function to execute
 * @param arg The argument to pass to the function
 */
void threadpool_add_task(threadpool *pool, void (*func)(void *), void *arg);

/**
 * @brief Destroy and free the threadpool
 * @param pool The threadpool to destroy
 */
void threadpool_destroy(threadpool *pool);

#endif  // THREADP_H