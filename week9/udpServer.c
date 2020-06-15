#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>

#define MAX_CONN 8

int InitUdpServer()
{
    int listen_fd, ret;
    int i = 1;
    struct sockaddr_in, server_addr;
    int sock_size = sizeof(struct sockaddr_in);
    listen_fd=socket(AF_INET,SOCK_STREAM,0);
    if (listen_fd < 0) {
		perror("Failed to create socket : ");
		return -1;
	}
    bzero(&server_addr,sock_size);
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(7000);
	server_addr.sin_addr.s_addr=INADDR_ANY;
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(int));
	ret = bind(listen_fd, (struct sockaddr*)&server_addr, sock_size);
    if (ret == 0){
		printf("Bind Successfully!\n");
        return listen_fd;
	} else {
        return -1;
    }
}

int main(int argc char **argv)
{
    int listen_fd, comm_fd, ret;
    char buff[512];
    listen_fd = InitUdpServer();
    if (listen_fd == -1){
        perror("Failed to init : ");
        return -1;
    }

    ret = listen(listen_fd, MAX_CONN);
    if (ret == 0) {
		printf("Listen Successfully!\n");
	}

    while(recvfrom(listen_fd, buff, ) > 0) {

    }
}