#include"threadpool.h"
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
//#include<pthread.h>
#if 0
typedef struct node{
  void *(callback)(void *args);
  void *args;
  struct node *next;

}node_t;


typedef struct{
  pthread_cond_t cond;
  pthread_mutex_t mutex;

  node_t *head;
  node_t *tail;
  int max_thread;
  int counter;
  int idle;
  int quit;
}threadpool_t;
#endif
void threadpool_init(threadpool_t *pool,int max_thread){
  pthread_cond_init(&(pool->cond),NULL);
  pthread_mutex_init(&(pool->mutex),NULL);
  pool->head=NULL;
  pool->tail=NULL;
  pool->max_thread=max_thread;
  pool->idle=0;
  pool->quit=0;
  pool->counter=0;
}
void *routine(void* arg)
{
threadpool_t *pool=(threadpool_t*)arg;
while(1){
  pthread_mutex_lock(&(pool->mutex));
  pool->idle++;
  while(pool->head==NULL&&pool->quit==0)
  {
    pthread_cond_wait(&(pool->cond),&(pool->mutex));
  }
  pool->idle--;
  if(pool->head!=NULL){
    node_t *tmp=pool->head;
   //此时可以解锁 
	pool->head = tmp->next;

    pthread_mutex_unlock(&pool->mutex);
    tmp->callback(tmp->args);
	free(tmp);
    pthread_mutex_lock(&pool->mutex);
  }
  if(pool->quit==1&&pool->head==NULL)
  {
    pool->counter--;
	if (pool->counter == 0) {
		pthread_cond_signal(&(pool->cond));
	}
    pthread_mutex_unlock(&(pool->mutex));
    break;
  }
  pthread_mutex_unlock(&(pool->mutex));
}
}
void threadpool_add_task(threadpool_t *pool,void *(task)(void*),void *arg)
{
  node_t *newnode=(node_t*)malloc(sizeof(node_t));
  memset(newnode,0x00,sizeof(node_t));
  newnode->callback=task;
  newnode->args=arg;

  pthread_mutex_lock(&(pool->mutex));
  if(pool->head==NULL){
    pool->head=newnode;
  }else{
    pool->tail->next=newnode;

  }
  pool->tail=newnode;
  if(pool->idle>0){
	  pthread_cond_signal(&(pool->cond));

  }else if(pool->counter<pool->max_thread){
    pthread_t tid;
    pthread_create(&tid,NULL,routine,(void*)pool);
   pthread_detach(tid);
    pool->counter++;
  }
  else{}
  pthread_mutex_unlock(&(pool->mutex));

}
void threadpool_destory(threadpool_t *pool)
{
  if(pool->quit==1)
    return ;
  pthread_mutex_lock(&pool->mutex);
  pool->quit=1;
  if(pool->counter>0){
    if(pool->idle){
  pthread_cond_broadcast(&(pool->cond));

    }//阻塞在这里等待所有线程销毁，才离开、
    while(pool->counter>0)
    pthread_cond_wait(&(pool->cond),&(pool->mutex));
  }
  pthread_mutex_unlock(&pool->mutex);

  pthread_cond_destroy(&(pool->cond));
  pthread_mutex_destroy(&(pool->mutex));
}


