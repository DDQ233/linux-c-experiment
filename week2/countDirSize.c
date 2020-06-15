#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc,char *argv[])
{
	DIR *dp;
	struct dirent *pDirent;
	int totalSize = 0;
	if (argc == 1) {
		dp=opendir(".");
	}
	else {
		dp=opendir(argv[1]);
	}
	
	while((pDirent=readdir(dp))!=NULL) {
		if ((strcmp(pDirent->d_name, ".") == 0) || (strcmp(pDirent->d_name, "..") == 0))
			continue;
		printf("%s\t",pDirent->d_name);
		
		int file = open(pDirent->d_name, O_RDONLY);
		int size = lseek(file, 0L, SEEK_END);
		printf("%d\n", size);
		totalSize += size;
		close(file);
	}
	
	printf("\n%d\n", totalSize);
	closedir(dp);
	return 0;
}
