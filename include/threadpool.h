#ifndef __THREAD_H__
#define __THREAD_H__
#include<pthread.h>

typedef struct node{
  void *(*callback)(void *args);
  void *args;
  struct node *next;

}node_t;


typedef struct{
  pthread_cond_t cond;
  pthread_mutex_t mutex;

  node_t *head;//任务队列头
  node_t *tail;//任务队列尾
  int max_thread;//线程最多允许创建的线程个数
  int counter;//线程池中当前线程个数
  int idle;//任务个数、
  int quit;
}threadpool_t;

void threadpool_init(threadpool_t *pool,int max_thread);

void threadpool_add_task(threadpool_t *pool,void *(task)(void*),void *arg);

void threadpool_destory(threadpool_t *pool);


#endif //__THREAD_H___
