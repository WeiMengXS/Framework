#ifndef __GLOBAL_H__
#define __GLOBAL_H__
typedef struct connection{
    int fd;
    int events;
    void (*rdhandler)(struct connection *c);
    void (*wdhandler)(struct connection *c);
    char buffer[2000];
    struct connection *data;
}connection_t;
int listen_init();
int epoll_init();
void epoll_process();
#endif //__GLOBAL_H__
