#ifndef __MYSQLUTIL_H__
#define __MYSQLUTIL_H__

MYSQL connectMysql(char *addr, char *username, char *pwd, char *database);
MYSQL_RES* query(MYSQL com_mysql, char *sql);
void freeResult(MYSQL_RES *pRes);
void close(MYSQL *com_mysql);

#endif