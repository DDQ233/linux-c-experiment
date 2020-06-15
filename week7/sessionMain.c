#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#define MAXCONN 8

struct session_message {
    int comm_fd;
    char ipaddr[16];
};

void *session_thread(void *arg)
{
    struct session_message *p = (struct session_message*)arg;
    int comm_fd = p->comm_fd;
    char ipaddr[16];
    sprintf(ipaddr, "%s", p->ipaddr);
    free(p);

    // Process
    while(1) {
        char buff[512];
        int count;
        count = read(comm_fd, buff, 511);
        if(count > 0) {
            buff[count] = 0;
            printf("A String from %s : %s\n", ipaddr, buff);
            if(strncmp(buff, "quit", 4) == 0) {
                printf("A talking is over!\n");
                break;
            }
            write(comm_fd, buff, count);
        } else {
            printf("A talking is over!\n");
            break;					
        }
    }
    pthread_exit(0);
}

int main()
{
    int listen_fd, comm_fd, ret;
    int i = 1;
	struct sockaddr_in server_addr,client_addr;
    // pthread_t tid[MAXCONN];

	int sock_size=sizeof(struct sockaddr_in);
    // Bind listening address and port
    bzero(&server_addr, sock_size);
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(8000);
	server_addr.sin_addr.s_addr=INADDR_ANY;
	listen_fd=socket(AF_INET,SOCK_STREAM,0);
    // The result of listen socket
	if (listen_fd < 0)
	{
		perror("Failed to create socket:");
		return -1;
	}
    ret=bind(listen_fd, (struct sockaddr*)&server_addr, sock_size);
    // The result of bind socket address
	if (ret == 0) {
		printf("Bind Successfully!\n");
	}
	ret = listen(listen_fd,MAXCONN);
	if (ret == 0)
	{
		printf("Listen Successfully!\n");
	}

    // Process
    while((comm_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &sock_size)) >= 0) {
        int err;
		char ipaddr[16];
		inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ipaddr, 16);
        pthread_t tid;
        struct session_message *pArg = (struct session_message*) malloc(sizeof(struct session_message));
        pArg->comm_fd = comm_fd;
        strncpy(pArg->ipaddr, ipaddr, 16);
        if ((err = pthread_create(&tid, NULL, session_thread, (void*)pArg)) != 0) {
            perror("pthread_create error");
        }
		printf("A connection come on:%s\n", ipaddr);
    }
	close(listen_fd);
	return 0;
}