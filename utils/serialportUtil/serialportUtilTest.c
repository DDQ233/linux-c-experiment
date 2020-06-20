#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include "serialportUtil.h"

int main()
{
    int uartFd;
	int count = 0;
    char buffer[]="Hello world!\r\n";
	char receiveBuffer[128];

    uartFd = UartOpen("/dev/ttyS0");
    uartFd = UartBindOptions(
        uartFd, 
        115200, 
        0, 
        8, 
        1, 
        'N');
    
    while(1) {
        /*
        write(uartFd, buffer, strlen(buffer));
        sleep(1);
        */
        printf("> Wait for data.\n\n");
        count = read(uartFd, receiveBuffer, 128);
        if (count > 0) {
            receiveBuffer[count]=0;
            printf("> STR : %s\n", receiveBuffer);
			count=0;
        }
    }
    close(uartFd);
    return 0;
}