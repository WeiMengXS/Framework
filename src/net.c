#include<unistd.h>
#include "global.h"
#include<sys/epoll.h>
#include<sys/socket.h>
#include<stdio.h>
#include "threadpool.h"
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#if 0
typedef struct connection{
    int fd;
    int events;
    void (*rdhandler)(struct connection *c);
    void (*wdhandler)(struct connection *c);
    char buffer[2000];
    struct connection *data;
}connection_t;

#endif
#define CONNSIZE 1024
int lfd=0;
int epfd=0;
connection_t connections[CONNSIZE];
connection_t *free_connection=NULL;
threadpool_t pool;
struct epoll_event evts[CONNSIZE];
void event_accept();
void epoll_add_event(int fd,int rdev,int wrev,int evtype,connection_t* c);
void wait_requst_handler();
void write_requst_handler();
int listen_init(){
  int lfd=socket(AF_INET,SOCK_STREAM,0);
  int op=1;
  setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&op,sizeof(op));
  struct sockaddr_in addr;
  addr.sin_family=AF_INET;
  addr.sin_port=htons(9898);
  addr.sin_addr.s_addr=htons(INADDR_ANY);
  int r=bind(lfd,(struct sockaddr*)&addr,sizeof(addr));
  listen(lfd,SOMAXCONN);
  return lfd;
}
connection_t *get_connection(int fd)
{
  connection_t *c=free_connection;
  free_connection=c->data;
  c->fd=fd;
  return c;
}

void close_connection(connection_t *c)
{
  close(c->fd);
  c->fd=-1;
  c->data=free_connection;
  free_connection=c;
}

int epoll_init()
{
    epfd=epoll_create1(EPOLL_CLOEXEC);
    threadpool_init(&pool,2);
    int i=CONNSIZE;

    connection_t *c=connections;
    connection_t *next=NULL;
    do{
      i--;
      c[i].data=next;
      c[i].fd=-1;
      next=&c[i];
    
      }while(i);
    free_connection=next;
    c=get_connection(lfd);
    c->rdhandler=event_accept;//监听套接字对应的回调函数
    epoll_add_event(c->fd,1,0,EPOLL_CTL_ADD,c);
}
void event_accept(connection_t *oc)
{
  int newfd=accept(oc->fd,NULL,NULL);
  connection_t *nc=get_connection(newfd);
nc->rdhandler=wait_requst_handler;
nc->wdhandler=write_requst_handler;

epoll_add_event(nc->fd,1,0,EPOLL_CTL_ADD,nc);
}


void epoll_add_event(int fd,int rdev,int wrev,int evtype,connection_t* c)
{
  struct epoll_event ev;
  if(evtype==EPOLL_CTL_MOD)
  {
    epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&ev);
    ev.data.ptr=c;
    ev.events=c->events;
    if(c->events==EPOLLIN)
    {
      ev.events=EPOLLOUT;
      c->events=EPOLLOUT;
    }else if(c->events==EPOLLOUT)
    {
      ev.events=EPOLLIN;
      c->events=EPOLLIN;
    }
    epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev);
  }else
  {
    if(rdev==1)
    {
    ev.events=EPOLLIN;
    c->events=EPOLLIN;
    }
    else if(wrev==1)
    {
      ev.events=EPOLLOUT;
      c->events=EPOLLOUT;
    }
    ev.data.ptr=c;
    epoll_ctl(epfd,evtype,fd,&ev);
  }
}
void* run(void* arg)
{
  connection_t* c=arg;
  int i=0;
  for(i=0;c->buffer[i];i++)
  {
    if(c->buffer[i]>='a'&&c->buffer[i]<='z')
      c->buffer[i]=c->buffer[i]-32;
  }
  epoll_add_event(c->fd,0,0,EPOLL_CTL_MOD,c);
}
void wait_requst_handler(connection_t* c)
{
  int r=read(c->fd,c->buffer,2000);
  if(r<=0)
  {
    epoll_ctl(epfd,EPOLL_CTL_DEL,c->fd,0);
    close_connection(c);
  }
  threadpool_add_task(&pool,run,c);
}
void write_requst_handler(connection_t* c)
{
  write(c->fd,c->buffer,strlen(c->buffer));
  epoll_add_event(c->fd,0,0,EPOLL_CTL_MOD,c);
}
//获取发生时:间

void epoll_process()
{
  int ready=epoll_wait(epfd,evts,CONNSIZE,-1);
  int i;
  connection_t *c=NULL;

  for(i=0;i<ready;i++)
  {
    int revents=evts[i].events;
    c=evts[i].data.ptr;
    if(revents&EPOLLIN)
    {
      (c->rdhandler)(c);
    }
    if(revents&EPOLLOUT)
    {
      (c->wdhandler)(c);
    }
  }
}
