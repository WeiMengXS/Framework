#ifndef __FUN_H__
#define __FUN_H__
int init_signal();
void init_proctitle();
void set_proctitle(const char *name);
void master_process_cycle();
int set_daemon();
#endif //__FUN_H__
