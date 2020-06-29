#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
int main(int argc,char **argv)
{
	int client_fd;
	int ret;
	int count;
	struct sockaddr_in server_addr;
	char buf[512];
	char recv_buf[512];
	int sock_size=sizeof(struct sockaddr_in);
	if(argc<2)
	{
		printf("> Usage:./client serverip\n");
		return 0;
	}
	bzero(&server_addr,sock_size);
	client_fd=socket(AF_INET,SOCK_STREAM,0);
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(8000);
	server_addr.sin_addr.s_addr=inet_addr(argv[1]);
	ret=connect(client_fd,(struct sockaddr*)&server_addr,sock_size);
	if(ret<0)
	{
		perror("> x Failed to connect : ");
		return -1;
	}
	printf("> O Connect successfully!\n");
	while(1)
	{
		fgets(buf,512,stdin);
		ret=write(client_fd,buf,strlen(buf));
		if(ret<=0)
			break;
		if(strncmp(buf,"quit",4)==0)
			break;	
		count=read(client_fd,recv_buf,511);
		if(count>0)
		{
			recv_buf[count]=0;
			printf("\nEcho ---> %s\n",recv_buf);
		}	
		else
		{
			break;
		}
	}
	close(client_fd);
	return 0;
	
}
