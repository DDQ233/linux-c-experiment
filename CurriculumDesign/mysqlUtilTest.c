#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>
#include "mysqlutil.h"

int main()
{
    // MySql connector
    MYSQL com_mysql;
    // SQL
    char sql[100];
    // Result set
    MYSQL_RES *pRes;
    // row
    MYSQL_ROW hs;

    com_mysql = connectMysql("192.168.0.102", "ddq", "0619", "studytest");
    
    sprintf(sql, "select * from tb_user");
    printf("> O SQL : %s\n", sql);
    pRes = query(com_mysql, sql);

    while(hs = mysql_fetch_row(pRes)) {
        printf("> hs={username=%s, password=%s, id=%s}\n", hs[0], hs[1], hs[2]);
    }
    
    freeResult(pRes);
    close(&com_mysql);
}