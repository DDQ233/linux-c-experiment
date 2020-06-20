#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

int UartOpen(char* srialPort)
{
	int fd;
	// Open serial port
	fd = open(srialPort, O_RDWR|O_NONBLOCK|O_NOCTTY);
	if (fd < 0) {
		perror("> x Cannot open serial port.\n");
		return -1;
	}
	printf("> O Open serial port.");

	// Reset the way of opening to dev file
    if (fcntl(fd, F_SETFL, 0) < 0) {
		perror("> x Failed to fcntl.\n");
		return -1;
	}
	printf("> O fcntl.\n");

	return fd;
}

int UartBindOptions(
	int fd, 
	int baudrate, 
	int flowCtrl, 
	int databits, 
	int stopbits, 
	int parity)
{
	int i = 0;
	int speed_arr[] = { B115200, B57600, B19200, B9600, B4800, B2400 };
	int name_arr[] = { 115200, 57600, 19200, 9600, 4800, 2400};
	struct termios uart_cfg;

	// Set uart type as original pattern
	cfmakeraw(&uart_cfg);
	printf("> O Set uart type as original pattern.\n");

	// Activate local connection
	uart_cfg.c_cflag |= CLOCAL;
	// Recevicing enabled
	uart_cfg.c_cflag |= CREAD;
	printf("> O Activate local connection and recevicing enabled.\n");

	// Set baudrate
	for(i = 0; i < sizeof(speed_arr) / sizeof(int); i++) {
		if (baudrate == name_arr[i]) {
			cfsetspeed(&uart_cfg, speed_arr[i]);
		}
	}
	printf("> O Set baudrate.\n");

	// Set flow control
	switch(flowCtrl) {
		// None
		case 0:
			uart_cfg.c_cflag &= ~CRTSCTS;
			break;
		// Hardware flow contorl
		case 1:
			uart_cfg.c_cflag |= CRTSCTS;
			break;
		// Software flow control
		case 2:
			uart_cfg.c_cflag |= IXON|IXOFF|IXANY;
			break;
		default:
			printf("> x Unsupport flow control.\n");
			return -1;
	}
	printf("> O Set flow control.\n");

	// Shielding other flag bit
	uart_cfg.c_cflag &= ~CSIZE;
	// Set data bit
	switch(databits) {
		case 5:
			uart_cfg.c_cflag |= CS5;
			break;
		case 6:
			uart_cfg.c_cflag |= CS6;
			break;
		case 7:
			uart_cfg.c_cflag |= CS7;
			break;
		case 8:
			uart_cfg.c_cflag |= CS8;
			break;
	}
	printf("> O Set data bit.\n");

	// Set parity check
	switch(parity) {
		// None
		case 'n':  
		case 'N':
			uart_cfg.c_cflag &= ~PARENB;
			uart_cfg.c_iflag &= ~INPCK;
			break;
		// Odd
		case 'o':
		case 'O':
			uart_cfg.c_cflag |= (PARODD | PARENB);
			uart_cfg.c_iflag |= INPCK;
			break;
		// Even
		case 'e':
		case 'E':
			uart_cfg.c_cflag |= PARENB;
			uart_cfg.c_cflag &= ~PARODD;
			uart_cfg.c_iflag |= INPCK;
			break;
		// Space
		case 's':
		case 'S':
			uart_cfg.c_cflag &= ~PARENB;
			uart_cfg.c_cflag &= ~CSTOPB;
			break;
		default:
			printf("> x Unsupport parity.\n");
			return -1;
	}
	printf("> O Set parity check.\n");

	// Set sopt bit
	switch(stopbits) {
		case 1:
			uart_cfg.c_cflag &= ~CSTOPB;
			break;
		case 2:
			uart_cfg.c_cflag |= CSTOPB;
			break;
		default:
			printf("> x Unsupport parity.\n");
			return -1;
	}
	printf("> O Set sopt bit.\n");

	// Clear buffer zone
	tcflush(fd, TCIOFLUSH);

	// Activate uart settings
	if (tcsetattr(fd, TCSANOW, &uart_cfg) != 0) {
		perror("> x Failed to activate uart settings.\n");
		return -1;
	}
	printf("> O Activate uart settings.\n");

	return fd;
}

/*
void sendData(int fd, char buffer[], int length)
{
	write(fd, buffer, length);
	printf("> Write data.\n");
}

char* readData(int fd)
{
	int count = 0;
	char receiveBuffer[128];

	count = read(fd, receiveBuffer, 128);
	if (count > 0) {
		receiveBuffer[count] = 0;
	}

	printf("> Read data.\n");

	return receiveBuffer;
}
*/