#ifndef __CON_POOL_H__
#define __CON_POOL_H_

#include<pthread.h>
#include"connection.h"

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
  node_t* head;
  node_t* tail;
  pthread_cond_t cond;
  pthread_mutex_t mutex;
}con_pool_t;

void con_pool_init(con_pool_t *pool,char *name,char *passwd,char *db,int maxsize);

void con_pool_push(con_pool_t *pool,char *sql);

#endif  //__CON_POOL_H__
