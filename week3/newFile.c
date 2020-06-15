#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	int file = open("1.txt", O_WRONLY);
	lseek(file, 3, SEEK_SET);
	write(file, "AB", 2);
	close(file);
	return 0;
}
