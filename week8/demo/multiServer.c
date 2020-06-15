#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <pthread.h>
#include <stdlib.h>
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
				printf("A String from %s:%s\n",ipaddr,buff);
				if(strncmp(buff,"quit",4)==0)
				{
					printf("A talking is over!\n");
					break;
				}
				write(comm_fd,buff,count);
			}
			else
			{
				printf("A talking is over!\n");
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
	server_addr.sin_port=htons(8000);
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
		struct session_message *pmessage=(struct session_message *)malloc(sizeof(struct session_message));
		pthread_t tid;
		inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,pmessage->ipaddr,16);
		printf("A connection come on:%s\n",pmessage->ipaddr);
		pmessage->comm_fd=comm_fd;
		pthread_create(&tid,NULL,session_thread,(void*)pmessage);			
	}
	close(listen_fd);
	return 0;
	
}
