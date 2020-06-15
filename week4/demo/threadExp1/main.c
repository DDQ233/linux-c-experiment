#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
void *thread_func(void *arg);
int main()
{
	int ret;
	int i;
	pthread_t tid[3];
  srand(time(NULL));
	for(i=0;i<3;i++)
	{
		ret=pthread_create(&tid[i],NULL,thread_func,(void*)i);
		if(ret<0)
			printf("Failed to child thread!\n");
	}
	printf("Succeeding in create three children thread.\nWating for them to finish!\n");
	for(i=0;i<3;i++)
	{
		ret=pthread_join(tid[i],NULL);
		if(ret<0)
		{
			printf("Child thread %d join failed!\n",i);
		}
		else
		{
			printf("Child thread %d joined!\n",i);	
		}
	}
	return 0;
}
