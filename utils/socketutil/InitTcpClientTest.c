#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "socketutil.h"

int main()
{
    int client_fd;
    char server_addr[] = "127.0.0.1";
    client_fd = initTcpClient(server_addr, 7000);
    closeConnect(client_fd);
    return 0;
}