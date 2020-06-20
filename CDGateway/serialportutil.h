#ifndef __SERIALPORTUTIL_H__
#define __SERIALPORTUTIL_H__

int UartOpen(char* srialPort);
int UartBindOptions(int fd, int baudrate, int flowCtrl, int databits, int stopbits, int parity);



// void sendData(int fd, char buffer[], int length);
// char* readData(int fd);

#endif