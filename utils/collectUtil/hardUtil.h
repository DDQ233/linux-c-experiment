#ifndef __HARDUTIL_H__
#define __HARDUTIL_H__

/**
 * Init serial port
 * 
 * @return File descriptor
 */
int initUart();

/**
 * Init mysql connector
 * 
 * @param *addr   Mysql host
 * @param *username   Username for login mysql
 * @param *pwd   Password for login mysql
 * @param *database   What database to connect
 * @return Database connection handle
 */
MYSQL connectMysql(char *addr, char *username, char *pwd, char *database);



#endif __HARDUTIL_H__