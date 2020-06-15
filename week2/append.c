#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	int file;
	int count;
	char str[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n";
	
	file = open("newFile.txt", O_RDWR|O_APPEND);
	if (file < 0) {
		perror("Append Failed.");
		close(file);
		return -1;
	}
	write(file, str, 27);
	printf("Write Finished.\n");
	close(file);
	return 0;
}
