#include<stdio.h>
#include<string.h>
#include<str.h>
#include<list.h>
#include<stdlib.h>
#include<config.h>
static list_t *head=NULL;
void load(const char *conf_name)
{
  if(conf_name==NULL)
    return ;

  FILE *fp=fopen(conf_name,"r");
  if(fp==NULL)
    return;
  char linebuf[500];
  head=list_init();
  while(!feof(fp))
  {
    memset(linebuf,0x00,sizeof(linebuf));
    //fgets(linebuf,500,fp);
    if(fgets(linebuf,500,fp)==NULL)
      continue;
    if(linebuf[0]=='\0')
      continue;
    if(linebuf[0]==' '||linebuf[0]=='\n'||linebuf[0]=='\r'||
      linebuf[0]=='#'||linebuf[0]=='\t')
lab:
    if(linebuf[strlen(linebuf)-1]==10||linebuf[strlen(linebuf)-1]==32||linebuf[strlen(linebuf)-1]==13)
    {
      linebuf[strlen(linebuf)-1]='\0';
      goto lab;
    }
    if(linebuf[0]=='\0')
      continue;
    if(linebuf[0]=='[')
      continue;
    char* ptmp=strchr(linebuf,'=');
    if(ptmp!=NULL)
    {
      char name[500]={};

      char value[500]={};
      strncpy(name,linebuf,(int)(ptmp-linebuf));
      strcpy(value,ptmp+1);
      ltrim(name);
      rtrim(name);
      ltrim(value);
      rtrim(value);
      list_insert(head,name,value);
    }
  }
  fclose(fp);
}
const char *get_string(const char *key){
node_t *tmp=head->head;
while(tmp!=NULL)
{
  if(strcmp(tmp->name,key)==0)
  {
    return tmp->value;
  }
  tmp=tmp->next;
}

}
int get_int_default(const char* key,const int def){
int ret=def;
node_t *tmp=head->head;
while(tmp!=NULL)
{
  if(strcmp(tmp->name,key)==0)
  {
    ret=atoi(tmp->value);
    break;
  }
  tmp=tmp->next;
}
return ret;
}

