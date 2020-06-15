#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>

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

void closeConnect(int fd)
{
    close(fd);
}

int login(int client_fd, char *username, char *pwd)
{
    int ret;
    ret = write(client_fd, username, strlen(username));
    if (ret <= 0 ) {
        printf("Write error");
        printf("username error");
        return 0;
    }
    ret = write(client_fd, password, strlen(password));
    if (ret <= 0 ) {
        printf("Write error");
        printf("password error");
        return 0;
    }
    printf("Login successfully !\n");
    return 1;
}

void runCmd(int client_fd, char *cmd)
{
    int length = strlen(cmd);
    cmd[length] = '\r';
    cmd[length] = '\n';
    
    ret = write(client_fd, cmd, strlen(cmd));

}

int main()
{
    int client_fd;
    int ret;
    int count;
    int sock_size = sizeof(struct sockaddr_in);
    struct socket_in server_addr;
    char addr[20];
    int port;
    char username[40];
    char password[40];
    char command[10];

    // get remote ftp server infomations
    printf("Init.................\n");
    // Host
    printf("Host : > ")
    fgets(strHost, 20, stdin);
    // fflush(stdout);
    // Port
    printf("Port : > ");
    scanf("%d", &port);
    // fflush(stdout);

    // connect remote ftp server
    client_fd = initTcpClient(addr, port);

    printf("--------------------------\n");
    // UserName
    printf("UserName : > ");
    // fflush(stdout);
    // getchar();
    fgets(username, 40, stdin);
    username[strlen(username) -1] = 0;
    // Password
    printf("password : > ", 40, stdin);
    fgets(password, 40, stdin);
    password[strlen(password) - 1] = 0;
    printf("---------------------------------------\n");

    // login to ftp
    if (login(client_fd, username, password) == 1 ) {
        printf("--------------------------------------\n\n")
        while(1) {
            printf("cmd : > ");
            fgets(command, 10, stdin);
            runCmd(client_fd, command);
            
        }
    } 
    else {
        return 0;
    }
}