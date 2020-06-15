#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	int file;
	int count;
	char str[] = "abcdefghijklmnopqrstuvwxyz\n";
	file = open("newFile.txt", O_CREAT, 777);
	close(file);
	file = open("newFile.txt", O_WRONLY);
	if ( file < 0) {
		perror("Failed to open file.");
		close(file);
		return -1;
	}
	write(file, str, 27);
	printf("Write finished.");
	close(file);
	return 0;
}

