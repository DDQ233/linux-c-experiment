#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int main()
{
	int file, fileLength;
	int fileWR;
	char *pmmap;
	char buf[1];

	file = open("12345.txt", O_RDWR);
	if (file < 0) {
		perror("Cannot open file : ");
		close(file);
		return -1;
	}
	
	fileLength = lseek(file, 0, SEEK_END);
	printf("File Length = %d\n\n", fileLength);
	pmmap = mmap(NULL, fileLength, PROT_READ|PROT_WRITE, MAP_SHARED, file, 0);
	if (pmmap == MAP_FAILED) {
		perror("Cannot map : ");
		close(file);
		return -1;
	}

	fileWR = open("12345.txt", O_RDWR);
	if (fileWR < 0) {
		perror("Cannot open file : ");
		close(fileWR);
		return -1;
	}
	// 文本文件内容字符数为奇数
	if ((fileLength - 1) % 2 == 1){
		printf("Reversing.....\n\n");
		int i;
		int j = fileLength - 1;
		printf("j = %d\n\n", j);
		// 中位数
		int mid = (fileLength - 1) / 2;
		// 临时变量
		char temp[1];
		// 字符串反转
		for (i = 0; i < mid; i++) {
			// 读取头尾字符
			read(fileWR, buf, 1);   // 读取头字符
			temp[0] = buf[0];   // temp[0] 存的是头字符
			printf("temp = %c\n", temp[0]);
			lseek(fileWR, j, SEEK_SET);
			read(fileWR, buf, 1);   // 读取尾字符，buf[0] 存的是尾字符
			printf("buf = %c\n", buf[0]);
			// 字符交换
			write(file, temp, 1);
			lseek(fileWR, i, SEEK_SET);
			write(file, buf, 1);
			
			j--;
		}
		munmap(pmmap, fileLength);
		close(fileWR);
		close(file);
		printf("\nFinished.\n\n");
	}

	// 文本文件内容字符数为偶数
	else if (fileLength % 2 == 0) {
	
	}
	return 0;	
}
