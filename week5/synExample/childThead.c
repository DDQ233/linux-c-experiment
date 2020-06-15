#include <semaphore.h>
#include <stdio.h>
extern sem_t isKey,isCal;
void *trd_func(void *arg)
{
	int x,y;
	while(1)
	{
		printf("Begin to wait child semaphore.\n");
		sem_wait(&isCal);
		printf("Succeed in getting child semaphore.\n");
		x=((int *)arg)[0];
		y=((int *)arg)[1];
		printf("x+y=%d\n",x+y);		
		sem_post(&isKey);
	}
}
