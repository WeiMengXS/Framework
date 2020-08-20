#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include"connection.h"
#include"con_pool.h"
#if 0
typedef struct node{
    MYSQL *conn;
    struct node *next;
}node_t;

typedef struct con_pool{
  char *name;
  char *passwd;
  char *db;
  int maxsize;
  int cursize;
  node* head;
  node* tail;
  pthread_cond_t cond;
  pthread_mutex_t mutex;
}con_pool_t;
#endif
void con_pool_init(con_pool_t *pool,char *name,char *passwd,char *db,int maxsize)
{
    pool->name=name;
    pool->passwd=passwd;
    pool->db=db;
    pool->maxsize=maxsize;
    pool->cursize=0;
    pool->head==NULL;
    pool->tail=NULL;
    pthread_cond_init(&(pool->cond),NULL);
    pthread_mutex_init(&(pool->mutex),NULL);

}

void con_pool_push(con_pool_t *pool,char *sql)
{
  pthread_mutex_lock(&(pool->mutex));
  if(pool->head!=NULL)
  {
    node_t *tmp=pool->head;
    pool->head=tmp->next;

    pthread_mutex_unlock(&(pool->mutex));
    update(tmp->conn,sql);
    pthread_mutex_lock(&(pool->mutex));

    tmp->next=pool->head;
    pool->head=tmp;

    pthread_cond_signal(&(pool->cond));
  }
  else if(pool->cursize<pool->maxsize)
  {
    node_t *tmp=malloc(sizeof(node_t));
    connection(&tmp->conn,pool->name,pool->passwd,pool->db);

    pthread_mutex_unlock(&(pool->mutex));
    update(tmp->conn,sql);
    
    pthread_mutex_lock(&(pool->mutex));
    tmp->next=pool->head;
    pool->head=tmp;

  }
  else
  {
    while(pool->head==NULL)
    {
      pthread_cond_wait(&(pool->cond),&(pool->mutex));
    }
    node_t *tmp=pool->head;
    pool->head=tmp->next;

    pthread_mutex_unlock(&(pool->mutex));
    update(tmp->conn,sql);
    
    pthread_mutex_lock(&(pool->mutex));

    tmp->next=pool->head;
    pool->head=tmp;

    pthread_cond_signal(&(pool->cond));
  }

    pthread_mutex_unlock(&(pool->mutex));
}

