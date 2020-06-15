#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>

int main(int argc, char **argv)
{
    int client_fd, ret, count;
    struct sockaddr_in server_addr;
    char buf[512];
    char recv_buf[512];

    // server address
    if (argc < 2) {
        printf("Usage:./client serverip\n");
        return 0;
    }
    int sock_size = sizeof(struct sockaddr_in);
    bzero(&server_addr, sock_size);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8000);
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    // Create client socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    // Connect server
    ret = connect(client_fd, (struct sockaddr*)&server_addr, sock_size);
    // The result of connecting server
	if (ret < 0)
	{
		perror("Failed to connect:");
		return -1;
	}
	printf("Connect successfully!\n");

    // Get string from keyborad , send string to server and receive string from server
    while(1) {
        fgets(buf, 512, stdin);
        ret = write(client_fd, buf, strlen(buf));
        // Failed to send string to server
        if (ret <=0) {
            break;
        }
        // Quit the program
        if (strncmp(buf, "quit", 4) == 0) {
            break;
        }
        // Receive string from server and output to the screen
        count = read(client_fd, recv_buf, 511);
        if (count > 0) {
            recv_buf[count] = 0;
            printf("Echo : %s \n\n", recv_buf);
        } else {
            break;
        }
    }

    close(client_fd);
    return 0;
}