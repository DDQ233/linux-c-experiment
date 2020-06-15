#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>

#define MAX_CONN 8

int main()
{
    int server_fd;
    char buf[512];
    struct sockaddr_in server_addr, client_addr;
    int count;

    server_fd = socket(AF_INET, SOCK_DGRAM, 0);         // create socket type of udp
    int sock_size = sizeof(struct sockaddr_in);
    bzero(&server_addr, sock_size);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(7000);
    bind(server_fd, (struct sockaddr*)&server_addr, sock_size);

    while((count = recvfrom(server_fd, buf, 512, 0, (struct sockaddr*)&client_addr, &sock_size)) > 0) {
        char ipStr[16];
        int port;
        inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ipStr, 16);
        port = ntohs(client_addr.sin_port);
        buf[count] = 0;
        if (strncmp(buf, "exit", 4) ==0 ){
            printf("Server closed");
            break;
        } else {
            printf("%s : %d , %s\r\n", ipStr, port);
        }
    }
    return 0;
}