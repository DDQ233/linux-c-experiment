#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

void *printThread1(void *arg);
void *printThread2(void *arg);

int main()
{
	int ret;
	pthread_t tid[2];
	
	ret = pthread_create(&tid[0], NULL, printThread1, NULL);
	if (ret < 0) {
		printf("Failed to child thread2 ! \n");
	}
	ret = pthread_create(&tid[1], NULL, printThread2, NULL);
	if (ret < 0) {
		printf("Failed to child thread2 ! \n");
	}
	ret = pthread_join(tid[0], NULL);
	if (ret < 0) {
		printf("Child thread1 join failed ! \n");
	}
	ret = pthread_join(tid[1], NULL);
	if (ret < 0) {
		printf("Child thread2 join failed ! \n");
	}
	return 0;
}
