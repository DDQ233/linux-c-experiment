#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>//要使用mmap必须包含次头文件
int main()
{
	int fd;
	int length;
	char *pmmap_buf;
	fd=open("1.txt",O_RDWR);
	if(fd<0)
	{
		perror("Cannot open file:");
		return -1;
	}
	length=lseek(fd,0,SEEK_END);//获取文件长度
	pmmap_buf=mmap(NULL,length,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);//映射文件
	if(pmmap_buf==MAP_FAILED)
	{
		perror("Cannot map:");
		close(fd);
	}
	*(pmmap_buf+3)='A';//修改文件的第3字节
	*(pmmap_buf+4)='B';	//修改文件的第4字节
	munmap(pmmap_buf,length);	//撤销映射
	close(fd);
}