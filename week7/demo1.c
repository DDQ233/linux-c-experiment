#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#define MAXCONN 8

struct session_message
{
    int comm_fd;
    char ipaddr[16];
};
void *session_thread(void *arg)
{
    struct session_message *p=(struct session_message*)arg; 
    int comm_fd=p->comm_fd;
    char ipaddr[16];
    sprintf(ipaddr,"%s",p->ipaddr);
    free(p);
while(1)
    {
        char buff[512];
        int count;
        count=read(comm_fd,buff,511);
        if(count>0)
        {
            buff[count]=0;
            printf("A string from %s:%s\n",ipaddr,buff);
            if(strncmp(buff,"quit",4)==0)
            {
                printf("Talking is over!\n");
                break;
            }
            write(comm_fd,buff,count);
        }
        else
        {
            printf("Talking is over!\n");
            break;
        }
    }
}

int main()
{
    int listen_fd,comm_fd;
    int ret;
    int i=1;
    struct sockaddr_in server_addr,client_addr;
    int sock_size=sizeof(struct sockaddr_in);
    listen_fd=socket(AF_INET,SOCK_STREAM,0);
    if(listen_fd<0)
    {
    perror("Failed to create socket:");
    return -1;
    }
    bzero(&server_addr,sock_size);
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(8080);
    server_addr.sin_addr.s_addr=INADDR_ANY;
    setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,&i,sizeof(int));
    ret=bind(listen_fd,(struct sockaddr*)&server_addr,sock_size);
    if(ret==0)
    {
        printf("Bind Successfully!\n");
    }
    ret=listen(listen_fd,MAXCONN);
    if(ret==0)
    {
        printf("Listen Successfully!\n");
    }
    while((comm_fd=accept(listen_fd,(struct sockaddr*)&client_addr,&sock_size))>=0)
    {
        char ipaddr[16];
        pthread_t tid;
        inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,ipaddr,16);
        printf("A connection come on:%s\n",ipaddr);
        //to thread
        struct session_message *pArg = (struct session_message*) malloc(sizeof(struct session_message));
        pArg->comm_fd=comm_fd;
        strcpy(pArg->ipaddr,ipaddr);
        pthread_create(&tid,NULL,session_thread,pArg);
        close(listen_fd);
    }
}
 
 

 