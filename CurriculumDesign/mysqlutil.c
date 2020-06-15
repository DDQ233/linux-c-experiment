#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>

MYSQL connectMysql(char *addr, char *username, char *pwd, char *database)
{
    MYSQL com_mysql;
    if (mysql_init(&com_mysql) == NULL) {
        printf("> x Cannot init mysql.\n");
        return com_mysql;
    }
    if(mysql_real_connect(&com_mysql, addr, username, pwd, database, 0, NULL, 0) == NULL) {
        printf("> x %s\n", mysql_error(&com_mysql));
        return com_mysql;
    }
    printf("> O MySql Connected.\n");
    return com_mysql;
}

MYSQL_RES* query(MYSQL com_mysql, char *sql)
{
    MYSQL_RES *pRes;
    if(mysql_query(&com_mysql, sql) != 0) {
        printf("> x %s\n", mysql_error(&com_mysql));
        return pRes;
    }
    if((pRes = mysql_store_result(&com_mysql)) == NULL) {
        printf("> x %s\n", mysql_error(&com_mysql));
        return pRes;
    }
    printf("> O Query successfully.\n");
    return pRes;
}

void freeResult(MYSQL_RES *pRes)
{
    mysql_free_result(pRes);
    printf("> Free result.\n");
}

void close(MYSQL *com_mysql)
{
    mysql_close(com_mysql);
    printf("> Close mysql connection.\n");
}
