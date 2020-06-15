#ifndef __SOCKETUTIL_H__
#define __SOCKETUTIL_H__

int initTcpServer(int port, int max_connect);
int initUdpServer(char *addr, int port);
int initTcpClient(char *addr, int port);
int initUdpClient(char *addr, int port);
void closeConnect(int fd);

#endif