#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
void *trd_func(void *arg);
sem_t isKey,isCal;
int main()
{
	pthread_t tid;
	int arg[2];
	sem_init(&isKey,0,1);
	sem_init(&isCal,0,0);
	pthread_create(&tid,NULL,trd_func,(void*)arg);
	while(1)
	{
		printf("Begin to wait main semaphore.\n");
		sem_wait(&isKey);
		printf("Succeed in getting main semaphore.\n");
		scanf("%d%d",&arg[0],&arg[1]);
		sem_post(&isCal);
	}
	sem_destroy(&isKey);
	sem_destroy(&isCal);
	return 0;
}
