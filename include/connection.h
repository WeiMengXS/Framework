#ifndef __CONNECTION_H__
#define __CONNECTION_H__
#include<mysql/mysql.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
//连接数据库
//连接数据库
int connection(MYSQL **my,char *name,char *passwd,char *db);
//更新数据库
int update(MYSQL *my,char *sql);
//查询数据库
MYSQL_RES *query(MYSQL *my,char *sql);

#endif //__CONNECTION_H__
