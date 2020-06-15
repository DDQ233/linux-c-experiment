#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
int main(int argc, char **argv)
{
  struct hostent *answer;
  int i;
  char ipstr[16];//为什么用16个字节？3*4+3+1=16，最多16个字节就是这样来的  
  if (argc != 2) 
  {
    printf("Usage: %s hostname\n",argv[0]);
    exit(1);    
  }
  answer = gethostbyname(argv[1]);
  if (answer == NULL) 
  {
    herror("gethostbyname"); //由gethostbyname自带的错误处理函数
    exit(1);
  }
  for (i = 0; (answer->h_addr_list)[i] != NULL; i++) 
  {
    inet_ntop(AF_INET, (answer->h_addr_list)[i], ipstr, 16);
    printf("%s\n", ipstr);
    printf("officail name : %s\n", answer->h_name);
  }
  return 0;
}