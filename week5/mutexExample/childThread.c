#include <pthread.h>//引用线程相关库函数必须包含次头文件
#include <stdio.h>
#include <stdlib.h>
#define DELAY_TIME_LEVELS 10.0
extern pthread_mutex_t mutex;
char *pmessage[]={"I am the first child","I am the second child","I am the third child"};
void *thread_func(void *arg)
{
	int  thread_num=(int)arg;
	int count;
	int delay_time;
	printf("Child thread %d is staring!\n",thread_num);
	pthread_mutex_lock(&mutex);
	for(count=0;count<5;count++)
	{
		printf("\t\tnum=%d,%s running for %d times.\n",thread_num,pmessage[thread_num],count);
		delay_time=(int)(rand()*DELAY_TIME_LEVELS/RAND_MAX)+1;
		sleep(delay_time);
	}
  pthread_mutex_unlock(&mutex);
	printf("Child thread %d finished\n",thread_num);
}
