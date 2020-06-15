#include <stdio.h>
#include <netinet/in.h>

int main()
{
    int i;
    unsigned int x = 0x12345678;
    unsigned char *p = (unsigned char*)&x;

    for (i = 0; i < 4; i++) {
        printf("%#x     ", p[i]);
    }

    printf("\n\n");
    
    return 0;
}