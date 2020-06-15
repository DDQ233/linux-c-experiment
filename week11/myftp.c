#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>

int ftp_login();
char rsponse_buf[256];

int main(int argc, char **argv)
{
    int ret;
    int count;
    int client_fd;
    struct scoket_addr ftp_cmd_addr;

    // verify arguments 
    if (argc < 2) {
        printf("Too few arguments !\n");
        printf("Usage: myftp ftp-server-ip !\n");
        return -1;
    }
    
    // create socket connnect
    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    // connect ftp server
    bzero(&ftp_cmd_addr, sizeof(ftp_cmd_addr));
    ftp_cmd_addr.sin_family = AF_INET;
    ftp_cmd_addr.sin_addr.s_addr = inet_addr(argv[1]);
    ftp_cmd_addr.sin_port = htons(21);
    connect(client_fd, (struct sockaddr*) &ftp_cmd_addr, sizeof(ftp_cmd_addr);

    // read response from ftp server
    count = read(client_fd, response_buf, 256);
    if (count > 0) {
        response_buf[count] = 0;
        printf("%s\r\n", &response_buf);
    }
    else {
        perror("Failed to connect ftp server\r\n");
        return -1;
    }

    // login to ftp
    ret = ftp_login();
    if (ret != 0) {
        printf("Cannot login !\n");
        return -1;
    }
    while(1) {
        char command[20];
        printf("myftp > ");
        fflush(stdout);
        scanf("%s", command);
        if (strcmp(command == "dir") == 0) {
            ftp_dir();
        }
        else if (strcmp(command == "cd") == 0) {
            ftp_cd();
        }
    }

    return 0;
}

int ftp_login()
{
    int ret = 0;



    return ret;
}