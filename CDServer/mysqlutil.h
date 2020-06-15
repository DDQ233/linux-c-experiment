#ifndef __MYSQLUTIL_H__
#define __MYSQLUTIL_H__

MYSQL connectMysql(char *addr, char *username, char *pwd, char *database);
void freeResult(MYSQL_RES *pRes);
void close(MYSQL *com_mysql);
MYSQL_RES* query(MYSQL com_mysql, char *sql);
MYSQL_RES* addData(MYSQL com_mysql);
MYSQL_RES* addDevice(MYSQL com_mysql);
MYSQL_RES* updateDevice(MYSQL com_mysql);
MYSQL_RES* deleteDevice(MYSQL com_mysql);
MYSQL_RES* findAllDevice(MYSQL com_mysql);
MYSQL_RES* findDeviceById(MYSQL com_mysql);

#endif