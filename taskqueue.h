#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include <pthread.h>
/**
 * @file taskqueue.h
 * @brief Task queue definitions
 */

typedef struct task {
    void (*func)(void *);
    void *arg;
    struct task *next;
} task;

typedef struct {
    task *front;
    task *rear;
    int len;
} taskqueue;

/**
 * @brief Create a new task
 * @param func The function to execute
 * @param arg The argument to pass to the function
 */
task *create_task(void (*func)(void *), void *arg);

/**
 * @brief Initialize a task queue
 * @param queue The task queue to initialize
 */
int taskqueue_init(taskqueue *queue);

/**
 * @brief Push a task to the task queue
 * @param queue The task queue to push to
 * @param task The task to push
 */
void taskqueue_push(taskqueue *queue, task *task);

/**
 * @brief Pop a task from the task queue
 * @param queue The task queue to pop from
 * @return The task popped from the queue
 */
task *taskqueue_pop(taskqueue *queue);

/**
 * @brief Clear the task queue
 * @param queue The task queue to clear
 */
void taskqueue_clear(taskqueue *queue);

/**
 * @brief Destroy and free the task queue
 * @param queue The task queue to destroy
 */
void taskqueue_destroy(taskqueue *queue);

#endif  // TASKQUEUE_H