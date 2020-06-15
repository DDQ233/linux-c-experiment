#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

typedef struct {
	int ThreadNum;
	int DelayTime;
	int StartCount;
	int EndCount;
} Args;

// arg = {ThreadNum, DelayTime, StartCount, EndCount}
void *thread_proc(void *arg)
{
	Args *var = (Args*) arg;
	int threadNum = var->ThreadNum;
	int sleeptime = var->DelayTime;
	int i = var->StartCount;
	int end = var->EndCount;
	printf("%d, %d, %d, %d\n\n", threadNum, sleeptime, i, end);
	while(i<=end){
		printf("thread%d : %d\n\n", threadNum, i);
		fflush(stdout);
		sleep(sleeptime);
		i++;
	}
	printf("Thread2 finished!\n");
	pthread_exit(NULL);
}

int main()
{
	Args thread1Args = {1, 4, 1, 10};
	Args thread2Args = {2, 2, 11, 20};
	pthread_t thread1, thread2;
	if(pthread_create(&thread1, NULL, thread_proc, (void*)&thread1Args)!=0){
		perror("Create thread1 failed");
	}
		if(pthread_create(&thread2, NULL, thread_proc, (void*)&thread2Args)!=0){
		perror("Create thread1 failed");
	}
	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);
}
