#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "log.h"
#include "taskqueue.h"

#define DEFAULT_THREAD_NUM 16

pthread_mutex_t mutexQueue;
pthread_cond_t condQueue;
taskqueue *queue;
struct arg_struct {
    int arg1;
    int arg2;
};

void thread_do() {
    while (1) {
        pthread_mutex_lock(&mutexQueue);
        while (queue->len == 0) {
            pthread_cond_wait(&condQueue, &mutexQueue);
        }
        task *mytask = taskqueue_pop(queue);
        pthread_mutex_unlock(&mutexQueue);
        uint64_t tid;
        pthread_threadid_np(NULL, &tid);
        struct arg_struct *myarg = mytask->arg;
        myarg->arg2 = tid;
        mytask->func(myarg);
        free(myarg);
        free(mytask);
    }
}

void mytasktask(void *arg) {
    sleep(1);
    int id = ((struct arg_struct *)arg)->arg1;
    int tid = ((struct arg_struct *)arg)->arg2;
    printf("Task %d by thread: %d\n", id, tid);
}

int main(int argc, char *argv[]) {
    pthread_t threads[DEFAULT_THREAD_NUM];
    queue = (taskqueue *)malloc(sizeof(taskqueue));
    taskqueue_init(queue);
    pthread_mutex_init(&mutexQueue, NULL);
    pthread_cond_init(&condQueue, NULL);
    int c = 0;
    for (int i = 0; i < DEFAULT_THREAD_NUM; i++) {
        pthread_create(&threads[i], NULL, (void *)thread_do, NULL);
    }
    // push 100 task to taskqueue
    for (int i = 0; i < 100; i++) {
        pthread_mutex_lock(&mutexQueue);
        struct arg_struct *arg = (struct arg_struct *)malloc(sizeof(struct arg_struct));
        arg->arg1 = i;
        arg->arg2 = 0;
        task *newTask = task_create(mytasktask, arg);
        taskqueue_push(queue, newTask);
        pthread_cond_signal(&condQueue);
        pthread_mutex_unlock(&mutexQueue);
    }

    // wait for all threads to finish
    // use join, but we can use detach too
    for (int i = 0; i < DEFAULT_THREAD_NUM; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_cond_destroy(&condQueue);
    pthread_mutex_destroy(&mutexQueue);
    taskqueue_destroy(queue);

    return 0;
}