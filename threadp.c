#define _GNU_SOURCE
#include <pthread.h>
#include <unistd.h>

#include "threadp.h"
#include "log.h"

void *thread_do(void *thread_p);

threadpool *threadpool_init(int num_of_threads) {
    threadpool *thpool = (threadpool *)malloc(sizeof(threadpool));
    if (thpool == NULL) {
        ERROR_MSG("[threadpool] Failed to initiate a threadpool");
        return NULL;
    }

    thpool->num_of_threads = 0;
    thpool->num_of_working_threads = 0;
    thpool->thread_alive = 1;
    thpool->queue = (taskqueue *)malloc(sizeof(taskqueue));
    taskqueue_init(thpool->queue);

    thpool->threads = (thread **)malloc(sizeof(thread *) * num_of_threads);

    if (thpool->threads == NULL) {
        ERROR_MSG("[threadpool] Failed to allocate memory for threads");
        taskqueue_destroy(thpool->queue);
        free(thpool);
        return NULL;
    }

    pthread_mutex_init(&thpool->mutexQueue, NULL);
    pthread_mutex_init(&thpool->mutexThreadCount, NULL);
    pthread_mutex_init(&thpool->mutexThreadLife, NULL);
    pthread_cond_init(&thpool->condQueue, NULL);
    pthread_cond_init(&thpool->condThreadCount, NULL);

    int i;
    for (i = 0; i < num_of_threads; i++) {
        thpool->threads[i] = (thread *)malloc(sizeof(thread));

        if (thpool->threads[i] == NULL) {
            ERROR_MSG("[threadpool] Failed to allocate memory for threads");
            taskqueue_destroy(thpool->queue);
            free(thpool->threads);
            free(thpool->queue);
            free(thpool);
            return NULL;
        }

        thpool->threads[i]->thpool = thpool;
        thpool->threads[i]->id = i;

        pthread_create(&(thpool->threads[i]->pthread), NULL, thread_do, (thpool->threads[i]));
        pthread_detach(thpool->threads[i]->pthread);
    }

    // waiting until all threads are ready
    while (thpool->num_of_threads != num_of_threads)
        ;

    return thpool;
}

void *thread_do(void *arg) {
    thread *thread_p = (thread *)arg;
    char thread_name[16] = {0};
    snprintf(thread_name, 16, "thpool-%d", thread_p->id);
    pthread_setname_np(thread_p->pthread, thread_name);

    threadpool *thpool = thread_p->thpool;
    taskqueue *queue = thpool->queue;

    pthread_mutex_lock(&thpool->mutexThreadCount);
    thpool->num_of_threads += 1;
    pthread_mutex_unlock(&thpool->mutexThreadCount);

    while (thpool->thread_alive) {
        pthread_mutex_lock(&thpool->mutexQueue);
        while (queue->len == 0) {
            pthread_cond_wait(&thpool->condQueue, &thpool->mutexQueue);
        }
        task *qtask = taskqueue_pop(queue);
        pthread_mutex_unlock(&thpool->mutexQueue);

        pthread_mutex_lock(&thpool->mutexThreadCount);
        thpool->num_of_working_threads += 1;
        pthread_mutex_unlock(&thpool->mutexThreadCount);

        LOG_MSG("[threadpool] thread: %s working on task", thread_name);
        qtask->func(qtask->arg);

        free(qtask->arg);
        free(qtask);

        pthread_mutex_lock(&thpool->mutexThreadCount);
        thpool->num_of_working_threads -= 1;
        pthread_mutex_unlock(&thpool->mutexThreadCount);
    }

    pthread_mutex_lock(&thpool->mutexThreadCount);
    thpool->num_of_threads -= 1;
    pthread_mutex_unlock(&thpool->mutexThreadCount);

    return NULL;
}

void threadpool_add_task(threadpool *thpool, void (*func)(void *), void *arg) {
    task *qtask = create_task(func, arg);

    pthread_mutex_lock(&thpool->mutexQueue);
    taskqueue_push(thpool->queue, qtask);
    pthread_mutex_unlock(&thpool->mutexQueue);
    pthread_cond_signal(&thpool->condQueue);
}

void threadpool_destroy(threadpool *thpool) {
    pthread_mutex_lock(&thpool->mutexThreadLife);
    thpool->thread_alive = 0;
    pthread_mutex_unlock(&thpool->mutexThreadLife);

    pthread_cond_broadcast(&thpool->condQueue);

    while (thpool->num_of_working_threads != 0) {
        usleep(10000);
    }

    taskqueue_destroy(thpool->queue);

    pthread_mutex_destroy(&thpool->mutexQueue);
    pthread_mutex_destroy(&thpool->mutexThreadCount);
    pthread_mutex_destroy(&thpool->mutexThreadLife);
    pthread_cond_destroy(&thpool->condQueue);
    pthread_cond_destroy(&thpool->condThreadCount);

    int i;
    for (i = 0; i < thpool->num_of_threads; i++) {
        free(thpool->threads[i]);
    }

    free(thpool->threads);
    free(thpool);
}