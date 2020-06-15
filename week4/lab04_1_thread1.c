#include <studio.h>
#include <stdlib.h>
#include <pthread.h>

#define DELAY_TIME = 4
#define COUNT_NUM = 10

void *printThread1(void *arg)
{
	for (int i = 1; i <= COUNT_NUM; i++) {
		printf("Thread_1 : %d\n\n", i);
		sleep(DELAY_TIME);
	}		
}
