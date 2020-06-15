#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
int main()
{
	int sockfd;
	int ret;
	char buff[512];
	char ipaddr[16];
	struct sockaddr_in server_addr,client_addr;
	int i=1;
	int sock_size=sizeof(struct sockaddr_in);
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0)
	{
		perror("Failed to socket:");
		return -1;
	}
	bzero(&server_addr,sock_size);
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(6000);
	server_addr.sin_addr.s_addr=INADDR_ANY;
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&i,sizeof(int));
  if(bind(sockfd,(struct sockaddr*)&server_addr,sock_size)<0)
	{
		perror("Failed to bind:");
		return -1;
	}
	while(1)
	{
		ret=recvfrom(sockfd,buff,512,0,(struct sockaddr*)&client_addr,&sock_size);
		if(ret>0)
		{
			buff[ret]=0;
			inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,ipaddr,16);
			printf("Receive a string from %s:%s\n",ipaddr,buff);
			sendto(sockfd,buff,ret,0,(struct sockaddr*)&client_addr,sock_size);
		}
	}
	close(sockfd);	
}
