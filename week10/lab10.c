#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>

int main()
{
    MYSQL com_mysql;
    MYSQL_RES *pRes;
    MYSQL_ROW hs;
    char sql[100];

    if(mysql_init(&com_mysql)==NULL){
        printf("Cannot init mysql!\n");
        return 0;
    }
    if(mysql_real_connect(&com_mysql,"192.168.0.102","root","root","studytest",0,NULL,0)==NULL){
        printf("%s\n",mysql_error(&com_mysql));
        return 0;
    }
    /*
    int id;
    char pwd[20];
    char name[20];

    id = 2;
    strcpy(pwd, "123321");
    strcpy(name, "123321");

    sprintf(sql, "insert into t_user(id, pwd, name) values(%d, %s, %s)", id, pwd, name);
    
    if(mysql_query(&com_mysql, sql) != 0){
        printf("%s\n",mysql_error(&com_mysql));
        return 0;
    }
    */

    mysql_close(&com_mysql);
    return 0;
}
