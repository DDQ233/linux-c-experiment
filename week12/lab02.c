#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

int main()
{
	int fd;
	int count=0;
	char receiveBuffer[128];
	char buffer[]="Hello world!\r\n";
	struct termios uart_cfg;

	// open serial dev file with the way of NIO
	fd = open("/dev/ttyS0", O_RDWR|O_NONBLOCK|O_NOCTTY);
	if(fd<0)
	{
		perror("Failed to open serial:");
		return -1;
	}

	// reset the way of opening to dev file
    fcntl(fd, F_SETFL,0);
	// set uart type as original pattern
	cfmakeraw(&uart_cfg);
	// Activate local connection and recevicing enabled
    uart_cfg.c_cflag |= CLOCAL|CREAD;
	// set baudrate
	cfsetspeed(&uart_cfg,B9600);
    // set 8 data bit
    uart_cfg.c_cflag &= ~CSIZE;
    uart_cfg.c_cflag |= CS8;
	// set even parity check
    uart_cfg.c_cflag |= PARENB;
    uart_cfg.c_cflag &= ~PARODD;
    uart_cfg.c_iflag |= INPCK;
    // set 2 stop bit
    uart_cfg.c_cflag |= CSTOPB;
	// clear buffer zone
	tcflush(fd, TCIOFLUSH);
	// activate uart settings
	tcsetattr(fd, TCSANOW, &uart_cfg);

	while(1)
	{
		write(fd,buffer,strlen(buffer));		
		sleep(1);
		/*
		printf("Wait for read:\r\n");
        count=read(fd,receiveBuffer,128);
        if(count>0)
        {
            receiveBuffer[count]=0;
            printf("STR:%s\r\n",receiveBuffer);
			// count=0;
        }
		*/
	}
	close(fd);
	return 0;
}