#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct session_message {
    int comm_fd;
    char ipaddr[16];
}

