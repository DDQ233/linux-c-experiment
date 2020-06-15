#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
sem_t can_add;//能够进行加法计算的信号量
sem_t can_scanf;//能够进行乘法计算的信号量
int x,y;
void *thread_add(void *arg)//加法线程入口函数
{
	while(1)
	{
		sem_wait(&can_add);//申请信号量
		printf("x+y=%d\n",x+y);
		sem_post(&can_scanf);//释放信号量
	}
}
int main()
{
	pthread_t tid;
	sem_init(&can_add, 0, 0);//初始化can_add信号量
	sem_init(&can_scanf, 0, 1);//初始化can_scanf信号量
	if(pthread_create(&tid,NULL,thread_add,NULL)<0)
	{
		printf("Create thread_add failed!\n");
		exit(0);
	}
	while(1)
	{
		sem_wait(&can_scanf);//申请信号量
		printf("Please input two integers:");
		scanf("%d%d",&x,&y);
		sem_post(&can_add);//初始化can_scanf信号量
	}		
}