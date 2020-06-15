#include <stdio.h>
#include <string.h>
#include <mysql.h>
int main(void)
{
    MYSQL com_mysql;//声明一个数据库连接句柄
    char sqlcommand[100];
    MYSQL_RES *pRes;
    MYSQL_ROW hs;
//MYSQL *mysql_init(MYSQL *mysql)
    if(mysql_init(&com_mysql)==NULL)
    {
            printf("Cannot init mysql!\n");
            return 0;
    }
    if(mysql_real_connect(&com_mysql,"192.168.0.1","jiabing",
                          "1234","computer",0,NULL,0)==NULL)
    {
        printf("%s\n",mysql_error(&com_mysql));
        return 0;
    }
//    sprintf(sqlcommand,"insert  into  student values( 88,40,'Mr Liu')");
//    if(mysql_query(&com_mysql,sqlcommand)!=0)
//    {
//        printf("%s\n",mysql_error(&com_mysql));
//        return 0;
//    }
    sprintf(sqlcommand,"select *  from student");
    if(mysql_query(&com_mysql,sqlcommand)!=0)
    {
        printf("%s\n",mysql_error(&com_mysql));
        return 0;
    }
    if((pRes=mysql_store_result(&com_mysql))==NULL)
    {
        printf("%s\n",mysql_error(&com_mysql));
        return 0;
    }
    while(hs=mysql_fetch_row(pRes))
    {
        //printf("id=%d,age=%d,name=%s\n",hs[0],hs[1],hs[2]);//错：因为mysql_fetch_row已经把字段值转换为字符串了
        printf("id=%s,age=%s,name=%s\n",hs[0],hs[1],hs[2]);
    }
    mysql_free_result(pRes);
    mysql_close(&com_mysql);
    return 0;
}

