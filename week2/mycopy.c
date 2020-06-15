#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
int main(int argc,char **argv)
{
	int fd_src,fd_dest;
	int count;
	unsigned char buffer[10];
	if(argc<3)
	{
		printf("太少的参数！\n用法：./mycopy  源文本文件名 目标文本文件名\n");
		return -1;
	}	
	fd_src=open(argv[1],O_RDONLY);
	if(fd_src<0)
	{
		perror("Failed to open src file");
		return -1;
	}
	fd_dest=open(argv[2],O_WRONLY|O_CREAT,0600);
	if(fd_src<0)
	{
		perror("Failed to dest src file");
		close(fd_src);
		return -1;
	}
	while((count=read(fd_src,buffer,10))>0)
	{
		write(fd_dest,buffer,count);//可以写成write(fd_dest,buffer,10)吗？
	}
	close(fd_src);
	close(fd_dest);
	return 0;
}
