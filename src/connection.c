
#include "connection.h"
//#include<mysql/mysql.h>
int connection(MYSQL **my,char *name,char *passwd,char *db)
{
  *my=mysql_init(NULL);
    if (mysql_real_connect(*my,"localhost",name,passwd,db,0,NULL,0)==NULL)
    {
      printf("mysql real connect %s",mysql_error(*my));
    }
}
//更新数据库
int update(MYSQL *my,char *sql)
{
  mysql_query(my,sql);
}
//查询数据库

