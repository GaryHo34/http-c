#ifndef TASKQUEUE_H
#define TASKQUEUE_H

/**
 * @file taskqueue.h
 * @brief Task queue definitions
 */

typedef struct task {
    void (*func)(void *);
    void *arg;
    struct task *next;
} task_t;

typedef struct taskqueue {
    task_t *front;
    task_t *rear;
    int size;
} taskqueue_t;

/**
 * @brief Create a new task
 * @param func The function to execute
 */
task_t *task_create(void (*func)(void *), void *arg);

/**
 * @brief Initialize a task queue
 * @param queue The task queue to initialize
 */
static int taskqueue_init(taskqueue_t *queue);

/**
 * @brief Push a task to the task queue
 * @param queue The task queue to push to
 * @param task The task to push
 */
static void taskqueue_push(taskqueue_t *queue, task_t *task);

/**
 * @brief Pop a task from the task queue
 * @param queue The task queue to pop from
 * @return The task popped from the queue
 */
static task_t *taskqueue_pop(taskqueue_t *queue);

/**
 * @brief Clear the task queue
 * @param queue The task queue to clear
 */
static void taskqueue_clear(taskqueue_t *queue);

/**
 * @brief Destroy and free the task queue
 * @param queue The task queue to destroy
 */
static void taskqueue_destroy(taskqueue_t *queue);

#endif  // TASKQUEUE_H