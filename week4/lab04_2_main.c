#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

int x = 0;
int y = 0;
int flag = 0;

void *thread_sum(void *arg)
{
	while(1){
		if(flag!=0){
			printf("Sum : %d\n\n", x + y);
			sleep(0.3);
			flag = 0;
		}
	}
}

int main()
{
	int ret;
	pthread_t tid;
	ret = pthread_create(&tid, NULL, thread_sum, NULL);
	if (ret < 0) {
		printf("Failed to create thread ! \n\n");
	}
	while(1) {
		printf("input two num : ");
		scanf("%d%d", &x, &y); 
		flag = 1;
		if (x == -1 && y == -1) {
			break;
		}
	}
	return 0;
}
