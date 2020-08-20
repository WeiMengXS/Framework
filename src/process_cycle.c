#define _GNU_SOURCE
#include<stdio.h>
#include<fun.h>
#include<config.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sched.h>
#include<string.h>
void start_worker_process(int nums,int type);
//主进程创建子进程
void spawn_process(int inum,char* procname,int type);
void worker_process_cycle(int inum,const char *procname);
void master_process_cycle()
{
  sigset_t set;
  sigemptyset(&set);

  sigaddset(&set,SIGHUP);
  sigaddset(&set,SIGINT);
  sigaddset(&set,SIGQUIT);
  sigaddset(&set,SIGABRT);
  sigaddset(&set,SIGBUS);
  sigaddset(&set,SIGUSR1);
  sigaddset(&set,SIGUSR2);
  sigaddset(&set,SIGSEGV);
  sigaddset(&set,SIGPIPE);
  sigaddset(&set,SIGALRM);
  sigaddset(&set,SIGTERM);
  sigaddset(&set,SIGCHLD);
  sigaddset(&set,SIGTTIN);
  sigaddset(&set,SIGTTOU);
  sigaddset(&set,SIGURG);
  sigaddset(&set,SIGWINCH);
  sigaddset(&set,SIGIO);
  sigprocmask(SIG_BLOCK,&set,NULL);

  int worker_process_num=get_int_default("WorkerCnt",1);
  start_worker_process(worker_process_num,-1);
 
  
  sigemptyset(&set);

  set_proctitle("framework:master");
  for(;;){
sigsuspend(&set);
printf("sigsuspend return");
  }
}
  
void start_worker_process(int nums,int type)
{
  int i;
  for(i=0;i<nums;i++)
  {
    spawn_process(i,"frameworker: process",type);
  }
}

void spawn_process(int inum,char* procname,int type)
{
  pid_t pid=fork();
  switch(pid){
    case -1:
      perror("frok"),exit(1);
    case 0:
      worker_process_cycle(inum,procname);
      break;
    default:
      break;
  }
}
static void worker_init(int inum)
{
  sigset_t set;
  sigemptyset(&set);
  sigprocmask(SIG_SETMASK,&set,NULL);
  cpu_set_t cpu;
  CPU_ZERO(&cpu);
  int cpu_count=get_int_default("CPUCNT",1);

 // printf("inum%%CPU_SETSIZE=%d\n",inum%cpu_count);
  CPU_SET(inum%cpu_count,&cpu);
  sched_setaffinity(0,sizeof(cpu),&cpu);

}
void worker_process_cycle(int inum,const char *procname)
{
worker_init(inum);
set_proctitle(procname);
for(;;)
{
  //epoll wait();
  sleep(1);
  printf("worker process %d\n",inum);
}
}


