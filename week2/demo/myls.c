#include <dirent.h>
#include <stdio.h>
int main(int argc,char *argv[])
{
	DIR *dp;
	struct dirent *pDirent;
	if(argc==1)
	{
		dp=opendir(".");
	}
	else
	{
		dp=opendir(argv[1]);
	}
	while((pDirent=readdir(dp))!=NULL)
	{
		if((strcmp(pDirent->d_name,".")==0)  || (strcmp(pDirent->d_name,"..")==0))
			continue;
		printf("%s\t",pDirent->d_name);
	}
	printf("\n");
	closedir(dp);
	return 0;
}
