#include <studio.h>
#include <stdlib.h>
#include <pthread.h>

#define DELAY_TIME = 2
#define COUNT_NUM = 20

void *printThread2(void *arg)
{
	for (int i =11; i <= COUNT_NUM; i++) {
		printf("Thread_2 : %d\n\n", i);
		sleep(DELAY_TIME);
	}		
}
