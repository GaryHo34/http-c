#include "taskqueue.h"

#include <stdlib.h>

#include "log.h"

task *create_task(void (*func)(void *), void *arg) {
    task *newTask = (task *)malloc(sizeof(task));
    if (newTask == NULL) {
        ERROR_MSG("[queue] Failed to allocate memory for a new task");
        return NULL;
    }
    newTask->func = func;
    newTask->arg = arg;
    newTask->next = NULL;
    return newTask;
}

int taskqueue_init(taskqueue *queue) {
    queue->front = NULL;
    queue->rear = NULL;
    queue->len = 0;
    return 0;
}

void taskqueue_push(taskqueue *queue, struct task *task) {
    if (queue->len == 0) {
        queue->front = task;
        queue->rear = task;
        queue->len = 1;
    } else {
        queue->rear->next = task;
        queue->rear = task;
        queue->len++;
    }
}

task *taskqueue_pop(taskqueue *queue) {
    if (queue->front == NULL)
        return NULL;
    if (queue->len == 1) {
        task *task = queue->front;
        queue->front = NULL;
        queue->rear = NULL;
        queue->len = 0;
        return task;
    }
    task *task = queue->front;
    queue->front = queue->front->next;
    queue->len--;
    return task;
}

void taskqueue_clear(taskqueue *queue) {
    while (queue->len) {
        task *task = taskqueue_pop(queue);
        free(task);
    }
}

void taskqueue_destroy(taskqueue *queue) {
    taskqueue_clear(queue);
    free(queue);
}
