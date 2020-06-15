#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "socketutil.h"

int main()
{
    int server_fd, comm_fd;
    struct sockaddr_in client_addr;
    int sock_size = sizeof(struct sockaddr_in);
    server_fd = initTcpServer(7000, 5);

    while((comm_fd = accept(server_fd, (struct sockaddr*)&client_addr, &sock_size)) >=0) {
        printf("accept\n");
    }
    return 0;
}