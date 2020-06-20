#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <termios.h>
#include <fcnlt.h>
#include <pthread.h>
#include <mysql/mysql.h>
#include "mysqlutil.h"
#include "mqttutil.h"
#include "serialport.h"

#define MYSQL_REMOTE_SERVER_ADDRESS "111.229.163.109"
#define MYSQL_REMOTE_SERVER_USERNAME "ddq"
#define MYSQL_REMOTE_SERVER_PASSWORD "0619"
#define MYSQL_REMOTE_SERVER_DATABASE "iot-device"
#define MYSQL_LOCAL_SERVER_ADDRESS "127.0.0.1"
#define MYSQL_LOCAL_SERVER_USERNAME "ddq"
#define MYSQL_LOCAL_SERVER_PASSWORD "0619"
#define MYSQL_LOCAL_SERVER_DATABASE "iot-device"

#define MQTT_SERVER_ADDRESS "tcp://111.229.163.109:1990"
#define MQTT_CLIENT_ID "Gateway"
#define MQTT_USERNAME "admin"
#define MQTT_PASSWORD "admin"
#define MQTT_MESSAGE_QOS 1
#define MQTT_TIMEOUT 10000L
#define MQTT_SENSOR_TOPIC_PERFIX "/sensor/"
#define MQTT_CONTORL_TOPIC_PERFIX "/ctrl/"
#define MQTT_KEEP_ALIVE_INTERVAL_TIME 20
#define MQTT_FLAG_CLEAN_SESSION 0
#define MQTT_FLAG_AUTOMATIC_RECONNECT 1

#define SERIALPORT_BAUDRATE 115200
#define SERIALPORT_FLOWCTRL 0
#define SERIALPORT_DATABITS 8
#define SERIALPORT_STOPBITS 1
#define SERIALPORT_PARITY 'N'

#define HALF_SECOND 0.5
#define ONE_SECOND 1
#define TWO_SECOND 2
#define FIVE_SECOND 5

#define UART_NUM 5
// #define UART_FD_PREFIX "/dev/ttyS"

static unsigned int isMysqlConnect 0;
static unsigned int isMqttConnect 0;
static unsigned int isAllSerialportConnect 0;
static int uartFdPool[UART_NUM];
static pthread_t serialportThreadPool[UART_NUM];
static char* serialportList[UART_NUM];
// static char* serialport[UART_NUM];
// static MYSQL mysqlClient;
// static MQTTAsync mqttclient;


void *serialportThread(void *arg)
{
    char receiveBuffer[128];
    int uartNum = (int)arg;
    int count = 0;
    while(1) {
        count = read(uartFdPool[uartNum], buffer, 128);
        if (count > 0) {
            printf("UART : %d > %s", i, receiveBuffer);

            /*
             * Do something 
             */

            count = 0;
        }
    }
}

// First run
void openSerialportList()
{
    int i = 0;
    printf("> O Opening serial port list.\n");
    for (i = 0; i < UART_NUM; i++) {
        char temp[15];
        sprintf(temp, "/dev/ttyS%d", i);
        serialportList[i] = temp;
        printf("> O Number of %d serial port was initialized.\n", i);
    }
    printf("> O Successfully.\n\n");
}

// Second run
void initSerialportList()
{
    int uartFd;
    printf("> O Initializing serial port list.\n");
    for (i = 0; i < UART_NUM; i++) {
        uartFdPool[i] = UARTOpen(serialportList[uartNum]);
        uartFdPool[i] = UartBindOptions(
            uartFdPool[i], 
            SERIALPORT_BAUDRATE, 
            SERIALPORT_FLOWCTRL, 
            SERIALPORT_DATABITS, 
            SERIALPORT_STOPBITS, 
            SERIALPORT_PARITY);
    }
    printf("> O Successfully.\n\n");
}

// Third run
void startSerialportThread()
{
    printf("> O Starting serial port thread.\n");
    int i = 0;
    int ret;
    for (i = 0; i < UART_NUM; i++) {
        uartFdPool[i] = pthread_create(&serialportThreadPool[i], NULL, serialportThread, (void*)i);
        if (uartFdPool[i] < 0) {
            printf("> x Failed to create number of %d pthread.\n", i);
        } else {
            printf("> O Number of %d thread was created.\n");
        }
    }
    printf("> O Successfully.\n\n");
}

int main()
{
    openSerialportList();
    initSerialportList();
    





    startSerialportThread();
}