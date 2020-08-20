#include<stdio.h>
#include<config.h>
#include<unistd.h>
#include<fun.h>
extern char* *environ;
char **g_os_argv;//命令行参数起始位置
int g_envmemlen;//环境变量大小
char* g_envmem;

int main(int argc,char*argv[])
{
  g_os_argv=argv;
  load("my.conf");
  init_proctitle();
//  set_proctitle("cdr");
  init_signal();


  if(get_int_default("Daemon",1)==1)
    set_daemon();

  master_process_cycle();

}
