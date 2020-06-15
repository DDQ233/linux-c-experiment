#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	int file;
	file = open("newFile.txt", O_TRUNC);
	if (file < 0) {
		perror("Trunc Failed.");
		close(file);
		return -1;
	}
	close(file);
	return 0;
}
