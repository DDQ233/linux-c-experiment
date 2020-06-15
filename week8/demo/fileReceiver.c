#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#define MAXCONN 8
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
	server_addr.sin_port=htons(7000);
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
		unsigned char buff[512];
		int length;//文件长度
		//接收文件名,简单起见，未考虑出错的情况
		int fd;
		int count=0;
		while(count<20)//直到收到２０个字节时为止
		{
			count+=read(comm_fd,buff,20-count);
		}
		buff[count]=0;
		printf("filename=%s\n",buff);
		fd=open(buff,O_RDWR|O_CREAT|O_TRUNC,0666);	//建立接收的文件
		 //接收文件长度
		 count=0;
		 while(count<4)
		 {
		 	count+=read(comm_fd,buff,4-count);
		 }	
		length=ntohl(*(unsigned int*)buff);
		printf("length=%d\n",length);	
		count=0;	 
		while(length>0)//接收文件字节
		{			
			count=read(comm_fd,buff,512<length?512:length);
			write(fd,buff,count);
			length-=count;			
		}
		close(fd);
	}
	close(listen_fd);
	return 0;
	
}
