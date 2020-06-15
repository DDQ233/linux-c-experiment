#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include "socketutil.h"

int initTcpServer(int port, int max_connect)
{
    int conn_num = 5;
    int server_fd, ret;
    int i = 1;
    struct sockaddr_in server_addr;
    int sock_size = sizeof(struct sockaddr_in);
    
    if (max_connect > 0) {
        conn_num = max_connect;
    }
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
		perror("Failed to create socket:");
		return -1;
	}
	bzero(&server_addr, sock_size);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(int));
	ret = bind(server_fd, (struct sockaddr*)&server_addr, sock_size);
    if(ret == 0) {
        printf("Bind Successfully!\n");
    } else {
        perror("Failed to bind");
        return -1;
    }
    ret = listen(server_fd, conn_num);
    if (ret == 0) {
        printf("Listen Successfully!\n");
        return server_fd;
    } else {
        return -1;
    }
}

int initUdpServer(char *addr, int port)
{
    
}

int initTcpClient(char *addr, int port)
{
    struct sockaddr_in server_addr;
    int client_fd;
    int ret;
    int sock_size = sizeof(struct sockaddr_in);
    // connect generation
    bzero(&server_addr, sock_size);
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(addr);
    ret = connect(client_fd, (struct sockaddr*)&server_addr, sock_size);
    // connect status
    if (ret < 0) {      // connect failed
        perror("Failed to connect : ");
        return -1;
    } else {        // connect successfully
        printf("Connect successfully.\n");
        return client_fd;
    }
}

int initUdpClient(char *addr, int port)
{

}

void closeConnect(int fd)
{
    close(fd);
}