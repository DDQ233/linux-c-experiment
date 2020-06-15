#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	int file, seekFile;
	int count;
	char buf[10];
	file = open("newFile.txt", O_RDONLY);
	if (file < 0) {
		perror("Read Failed.");
		close(file);
		return -1;
	}
	seekFile = lseek(file, -6, SEEK_END);
	if (seekFile < 0) {
		perror("Seek Failed.");
		close(file);
		return -1;
	}
	while((count = read(file, buf, 10)) > 0) {
		printf("String : %s\n", buf);
	}
	close(file);
	return 0;
}
