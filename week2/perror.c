#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	int fd;
	fd=open("lab01File.txt", O_RDWR);
	if (fd < 0) {
		// printf("Failed to open");
		perror("Failed to open");
		return -1;
	}
	close(fd);
	return 0;
}
