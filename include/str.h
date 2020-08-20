#include<string.h>
void ltrim(char *str)
{
  if(str==NULL)
    return ;
  int len=strlen(str);
  while(len>0&&str[len-1]==' ')
  {
    str[--len]=0;
  }
}
void rtrim(char *str)
{
  if(str==NULL)
    return ;
  int len=strlen(str);
  if(len<0)
    return ;
  char* ptmp=str;
  if(*ptmp!=' ')
    return;
  while(*ptmp!='\0')
  {
    if(*ptmp!=' ')
      break;
    ptmp++;
  }
  char *ptmp2=str;
  while(*ptmp!='\0')
    *ptmp2++=*ptmp++;
  *ptmp2='\0';
}
