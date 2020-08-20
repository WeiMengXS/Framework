#include<global.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
extern char** environ;
void init_proctitle(){
int i;
for(i =0;environ[i]!=NULL;i++)
  g_envmemlen+=strlen(environ[i])+1;

g_envmem=malloc(sizeof(char)*g_envmemlen);
memset(g_envmem,0x00,g_envmemlen);

char* tmp=g_envmem;
for(i=0;environ[i]!=NULL;i++)
{
  int len=strlen(environ[i])+1;
  strcpy(tmp,environ[i]);
  environ[i]=tmp;
  tmp+=len;
}
}
void set_proctitle(const char *name){
int title_len=strlen(name);
int arg_len=0;
int i;
for(i=0;g_os_argv[i]!=NULL;i++)
{
  arg_len+=strlen(g_os_argv[i])+1;
}
if(arg_len+g_envmemlen<title_len)
{
  return;
}
g_os_argv[1]=NULL;
char *tmp=g_os_argv[0];
strcpy(tmp,name);
tmp+=title_len;
memset(tmp,0x00,(arg_len+g_envmemlen)-title_len);
}
