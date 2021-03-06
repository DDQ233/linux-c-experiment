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
	int length;
	int fd;
	struct sockaddr_in server_addr;
	char buff[512];
	int sock_size=sizeof(struct sockaddr_in);
	if(argc<3)
	{
		printf("Usage:./client serverip filename\n");
		return 0;
	}
	bzero(&server_addr,sock_size);
	client_fd=socket(AF_INET,SOCK_STREAM,0);
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(7000);
	server_addr.sin_addr.s_addr=inet_addr(argv[1]);
	ret=connect(client_fd,(struct sockaddr*)&server_addr,sock_size);
	if(ret<0)
	{
		perror("Failed to connect:");
		return -1;
	}
	printf("Connect successfully!\n");
	fd=open(argv[2],O_RDWR);
	if(fd<0)
	{
		perror("Cannot open:");
			//break;
	}
       //发送文件名
		bzero(buff,512);
		strcpy(buff,argv[2]);
		ret=write(client_fd,buff,20);
		printf("name=%s\n",argv[2]);
		//if(ret<=0)
			//break;
       //发送文件长度
    length=lseek(fd,0,SEEK_END);
    length=htonl(length);
    write(client_fd,&length,4);
    	//发送文件字节
		lseek(fd,0,SEEK_SET);
    while(length>0)
    	{
    		count=read(fd,buff,512);
    		if(count>0)
    			{
    				write(client_fd,buff,count);
    				length-=count;
    			}
    		else
    				break;
    	}	
	close(client_fd);
	return 0;
	
}
