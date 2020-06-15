#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

sem_t can_scanf;
sem_t can_add;
sem_t can_multiply;
int x, y;

void *thread_add(void *arg)
{
	while(1)
	{
		sem_wait(&can_add);//申请信号量
		printf("x + y = %d\n",x + y);
		sem_post(&can_multiply);//释放信号量
	}
}

void *thread_multiply(void *arg)
{
	while(1)
	{
		sem_wait(&can_multiply);//申请信号量
		printf("x * y = %d\n",x * y);
		sem_post(&can_scanf);//释放信号量
	}
}

int main()
{
    pthread_t t_add, t_multiply;
    sem_init(&can_scanf, 0, 1);
    sem_init(&can_add, 0, 0);
    sem_init(&can_multiply, 0, 0);

    if (pthread_create(&t_add, NULL, thread_add, NULL) < 0) {
        printf("Craete thread_add failed ! \n");
        exit(0);
    }
    if (pthread_create(&t_multiply, NULL, thread_multiply, NULL) < 0) {
        printf("Craete thread_multiply failed ! \n");
        exit(0);
    }

    while(1) {
        sem_wait(&can_scanf);
        printf("Please input two integers:");
        scanf("%d%d", &x, &y);
        sem_post(&can_add);
    }

}