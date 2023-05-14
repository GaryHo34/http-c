#include "taskqueue.h"

#include <stdlib.h>

#include "log.h"

task_t *task_create(void (*func)(void *), void *arg) {
    task_t *newTask = (task_t *)malloc(sizeof(task_t));
    if (newTask == NULL) {
        ERROR_MSG("[queue] Failed to allocate memory for new task");
        return NULL;
    }
    newTask->func = func;
    newTask->arg = arg;
    return newTask;
}

static int taskqueue_init(taskqueue_t *queue) {
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return 0;
}

static void taskqueue_push(taskqueue_t *queue, task_t *task) {
    if (queue->size == 0) {
        queue->front = task;
        queue->rear = task;
        queue->size = 1;
    } else {
        queue->rear->next = task;
        queue->rear = task;
        queue->size++;
    }
}

static task_t *taskqueue_pop(taskqueue_t *queue) {
    if (queue->front == NULL)
        return NULL;
    if (queue->size == 1) {
        task_t *task = queue->front;
        queue->front = NULL;
        queue->rear = NULL;
        queue->size = 0;
        return task;
    }
    task_t *task = queue->front;
    queue->front = queue->front->next;
    queue->size--;
    return task;
}

static void taskqueue_clear(taskqueue_t *queue) {
    while (queue->size) {
        task_t *task = taskqueue_pop(queue);
        free(task);
    }
}

static void taskqueue_destroy(taskqueue_t *queue) {
    taskqueue_clear(queue);
    free(queue);
}

void task(void *arg) {
    printf("Task %d\n", *(int *)arg);
}


int main(int argc, char *argv[]) {
    taskqueue_t *queue = (taskqueue_t *)malloc(sizeof(taskqueue_t));
    taskqueue_init(queue);
    for (int i = 0; i < 10; i++) {
        task_t *newTask = task_create(task, &i);
        taskqueue_push(queue, newTask);
    }
    for (int i = 0; i < 10; i++) {
        task_t *task = taskqueue_pop(queue);
        task->func(task->arg);
        free(task);
    }
    taskqueue_destroy(queue);
    return 0;
}