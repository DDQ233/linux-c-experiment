#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int main()
{
	int file, fileLength;
	char *pmmap;
	char temp;
	int i, j;
	
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
	
	printf("Reversing......\n\n");
	if ((fileLength - 1) % 2 == 1) {
		j = fileLength - 1;
		int mid = (fileLength - 1 - 1) / 2;
		for (i = 0; i < mid; i++) {
			temp = *(pmmap + j);
			*(pmmap + j) = *(pmmap + i);
			*(pmmap + i) = temp;
			j--;
	
		}
	}
	else if ((fileLength - 1) % 2 == 0) {
		j = fileLength - 1;
		for (i = 0; i <= (fileLength - 1) / 2; i++) {
			temp = *(pmmap + j);
			*(pmmap + j) = *(pmmap + i);
			*(pmmap + i) = temp;
			j--;
		}
	}

	printf("Finished.\n");
	munmap(pmmap, fileLength);
	close(file);
	return 0;
}
