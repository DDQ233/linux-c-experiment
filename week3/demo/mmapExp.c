#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>//Ҫʹ��mmap���������ͷ�ļ�
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
	length=lseek(fd,0,SEEK_END);//��ȡ�ļ�����
	pmmap_buf=mmap(NULL,length,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);//ӳ���ļ�
	if(pmmap_buf==MAP_FAILED)
	{
		perror("Cannot map:");
		close(fd);
	}
	*(pmmap_buf+3)='A';//�޸��ļ��ĵ�3�ֽ�
	*(pmmap_buf+4)='B';	//�޸��ļ��ĵ�4�ֽ�
	munmap(pmmap_buf,length);	//����ӳ��
	close(fd);
}