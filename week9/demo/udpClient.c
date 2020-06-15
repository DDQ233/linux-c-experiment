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
	struct sockaddr_in server_addr,sock_addr;
	char buf[512];
	char recv_buf[512];
	int sock_size=sizeof(struct sockaddr_in);
	if(argc<2)
	{
		printf("Usage:./udpclient serverip\n");
		return 0;
	}
	client_fd=socket(AF_INET,SOCK_DGRAM,0);
	bzero(&server_addr,sock_size);
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(6000);
	server_addr.sin_addr.s_addr=inet_addr(argv[1]);
	while(1)
	{
		fgets(buf,512,stdin);
		ret=sendto(client_fd,buf,strlen(buf),0,(struct sockaddr*)&server_addr,sock_size);
		if(ret<0)
		{
			perror("Failed to sendto:");
			break;
		}
		if(strncmp(buf,"quit",4)==0)
			break;	
		count=recvfrom(client_fd,recv_buf,512,0,(struct sockaddr*)&sock_addr,&sock_size);
		if(count>0)
		{
			recv_buf[count]=0;
			printf("Echo:%s\n",recv_buf);
		}	
		else
		{
			perror("Failed to recvfrom:");
			break;
		}
	}
	close(client_fd);
	return 0;
	
}
