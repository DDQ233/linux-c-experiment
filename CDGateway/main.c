#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <pthread.h>
#include <time.h>
#include <MQTTAsync.h>
#include <mysql/mysql.h>
#include <cjson/cJSON.h>
#include "mysqlutil.h"
#include "mqttutil.h"
#include "serialportutil.h"

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

#define DEVICE_NUM 5
// #define UART_FD_PREFIX "/dev/ttyS"

static unsigned int isMysqlConnect = 0;
static unsigned int isMqttConnect = 0;
static unsigned int isAllSerialportConnect = 0;
static unsigned int deviceStatus[DEVICE_NUM];
static char sensorMqttTopic[DEVICE_NUM][20];
static int uartFdPool[DEVICE_NUM];
static pthread_t serialportThreadPool[DEVICE_NUM];
static MYSQL mysqlClient;
static MYSQL_RES *pRes;
static MQTTAsync mqttClientList[DEVICE_NUM];
// static char* pSerialport;
// static char serialportList[DEVICE_NUM][15];
// static char* serialport[DEVICE_NUM];
// static MYSQL mysqlClient;
// static MQTTAsync mqttclient;


void connlost(void *context, char *cause)
{

}

int messageArrived(void* context, char* topicName, int topicLen, MQTTAsync_message* m)
{

}

void onConnectFailure(void* context, MQTTAsync_failureData* response)
{

}

void onConnect(void* context, MQTTAsync_successData* response)
{

}

void onDisconnectFailure(void* context, MQTTAsync_failureData* response)
{

}

void onDisconnect(void* context, MQTTAsync_successData* response)
{

}

void onSendFailure(void* context, MQTTAsync_failureData* response)
{

}

void onSend(void* context, MQTTAsync_successData* response)
{

}



int main()
{
    // Function declaration
    void initDeviceStatus();
    void *serialportThread(void *arg);
    void initSerialportList();
    void startSerialportThread();
    void closeSerialport();
    void closeSerialportThread();
    void startMySqlService();
    void stopMysqlService();
    void startMqttService();
    void stopMqttService();

    // Function initialization
    initDeviceStatus();
    // openSerialportList();
    initSerialportList();
    startMySqlService();
    startMqttService();
    startSerialportThread();

    sleep(ONE_SECOND);

    // int i = 0;
    // int ret;
    // for (i = 0; i < DEVICE_NUM; i++) {
    //     ret = pthread_join(serialportThreadPool[i], NULL);
    //     if (ret < 0) {
    //         printf("> x Failed to join number of %d pthread.\n", i);
    //     } else {
    //         printf("> O Number of %d thread was joined.\n", i);
    //     }
    // }

    printf("> O Application running........\n");

    // closeSerialportThread();
    // closeSerialport();
    // stopMysqlService();
    // stopMqttService();

    while(1) {

    }

    return 0;
}

void initDeviceStatus()
{
    printf("> O Initializing all device status.\n");
    int i = 0;
    for (i = 0; i < DEVICE_NUM; i++) {
        deviceStatus[i] = 0;
    }
    printf("> O Finished.\n\n");
    sleep(HALF_SECOND);
}


void *serialportThread(void *arg)
{
    // Variables
    char receiveBuffer[128];
    int deviceNum = *((int*)arg);
    int count = 0;
    int ret;
    int fd = uartFdPool[deviceNum];
    cJSON *json, *json_value;
    MQTTAsync client = mqttClientList[deviceNum];
    MQTTAsync_message message = MQTTAsync_message_initializer;
    MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
    opts.onSuccess = onSend;
	opts.onFailure = onSendFailure;
	opts.context = client;
    printf("> O Thread %d running.\n", deviceNum);
    // Run
    while(1) {
        printf("> O Wait for data.......\n");
        count = read(fd, receiveBuffer, 128);
        if (count > 0) {
            printf("UART %d > %s", deviceNum, receiveBuffer);
            // Send message to mqtt server
            message.payload = receiveBuffer;
            message.payloadlen = (int)strlen(receiveBuffer);
            message.qos = MQTT_MESSAGE_QOS;
            // Device auth ? Device Status ?
            json = cJSON_Parse(receiveBuffer);
            json_value = cJSON_GetObjectItem(json, "uid");
            if (sensorMqttTopic[deviceNum] == NULL) {
                sprintf(sensorMqttTopic[deviceNum], MQTT_SENSOR_TOPIC_PERFIX);
                strcat(sensorMqttTopic[deviceNum], json_value->valuestring);
                printf("> Topic -----> %s", sensorMqttTopic[deviceNum]);
            }
            // Can topic make it static and save in memory ? 
            if ((ret = MQTTAsync_sendMessage(client, sensorMqttTopic[deviceNum], &message, &opts)) != MQTTASYNC_SUCCESS) {
                printf("> x Failed to send message. Error code : %d\n", ret);
            } else {
                printf("> Send -----> %s.\n", receiveBuffer);
            }
            memset(receiveBuffer, 0, sizeof(receiveBuffer));
            count = 0; 
        }
    }
}

// First run
// void openSerialportList()
// {
//     int i = 0;
//     printf("> O Opening serial port list.\n");
//     for (i = 0; i < DEVICE_NUM; i++) {
//         char temp[15];
//         sprintf(temp, "/dev/ttyS%d", i);
//         serialportList[i] = temp;
//         printf("> O Open : %s.\n", temp);
//         printf("> O Number of %d serial port was initialized.\n", i);
//     }
//     printf("> O Finished.\n\n");
// }

void initSerialportList()
{
    int i = 0;
    int uartFd;
    printf("> O Initializing serial port list.\n");
    for (i = 0; i < DEVICE_NUM; i++) {
        char serialport[15];
        sprintf(serialport, "/dev/ttyS%d", i);
        uartFdPool[i] = UartOpen(serialport);
        uartFdPool[i] = UartBindOptions(
            uartFdPool[i], 
            SERIALPORT_BAUDRATE, 
            SERIALPORT_FLOWCTRL, 
            SERIALPORT_DATABITS, 
            SERIALPORT_STOPBITS, 
            SERIALPORT_PARITY);
        if (uartFdPool[i] == -1) {
            printf("> x Failed to open serial port %d.\n", i);
        } else {
            printf("> O Serial port %d was opened.\n", i);
        }
    }
    printf("> O Finished.\n\n");
    sleep(HALF_SECOND);
}

void startSerialportThread()
{
    printf("> O Starting serial port thread.\n");
    int i = 0;
    int ret;
    for (i = 0; i < DEVICE_NUM; i++) {
        uartFdPool[i] = pthread_create(&serialportThreadPool[i], NULL, serialportThread, (void*)&i);
        if (uartFdPool[i] < 0) {
            printf("> x Failed to create number of %d pthread.\n", i);
        } else {
            printf("> O Number of %d thread was created.\n", i);
        }
        sleep(HALF_SECOND);
    }
    printf("> O Finished.\n\n");
    sleep(HALF_SECOND);
}

void closeSerialportThread()
{
    printf("> O Closing serial port thread.\n");
    int i = 0;
    int ret;
    ret = pthread_cancel(serialportThreadPool[i]);
    if (ret < 0) {
        printf("> x Failed to cancel number of %d pthread.\n", i);
    } else {
        printf("> O Number of %d thread was canceled.\n", i);
    }
    printf("> O Finished.\n\n");
    sleep(HALF_SECOND);
}

void startMySqlService()
{
    printf("> O Starting mysql service.\n");
    mysqlClient = connectMysql(
        MYSQL_REMOTE_SERVER_ADDRESS, 
        MYSQL_REMOTE_SERVER_USERNAME, 
        MYSQL_REMOTE_SERVER_PASSWORD, 
        MYSQL_REMOTE_SERVER_DATABASE);
    printf("> O Mysql service was started.\n\n");
    sleep(HALF_SECOND);
}

void startMqttService()
{
    printf("> O Starting mqtt service.\n");
    int i = 0;
    MQTTAsync_connectOptions conn_opts;
    MQTTAsync_responseOptions resp_opts;
    MQTTAsync_disconnectOptions disc_opts;
    conn_opts = bindConnectOptions(
        MQTT_USERNAME, 
        MQTT_PASSWORD, 
        MQTT_KEEP_ALIVE_INTERVAL_TIME, 
        MQTT_FLAG_CLEAN_SESSION, 
        MQTT_FLAG_AUTOMATIC_RECONNECT, 
        onConnect, 
        onConnectFailure);
    resp_opts = bindResponseOptions(onSend, onSendFailure);
    disc_opts = bindDisconnectOptions(onDisconnect, onDisconnectFailure);
    for (i = 0; i < DEVICE_NUM; i++) {
        mqttClientList[i] = createClient(MQTT_SERVER_ADDRESS, MQTT_CLIENT_ID);
        mqttClientList[i] = setCallbacks(mqttClientList[i], connlost, messageArrived, NULL);
        mqttClientList[i] = connectMqttServer(mqttClientList[i], conn_opts);
        printf("> O Number of %d mqtt client initialized.\n", i);
    }
    printf("> O Mqtt service was started.\n\n");
    sleep(HALF_SECOND);
}

void closeSerialport()
{
    printf("> O Closing serial port.\n");
    int i = 0;
    for (i = 0; i < DEVICE_NUM; i++) {
        close(uartFdPool[i]);
        printf("> O Number of %d serial port was closed.\n", i);
    }
    printf("> O Finished.\n\n");
    sleep(HALF_SECOND);
}

void stopMysqlService()
{
    printf("> O Stoping mysql service.\n");
    freeResult(pRes);
    mysql_close(&mysqlClient);
    printf("> O Finished.\n\n");
    sleep(HALF_SECOND);
}

void stopMqttService()
{
    printf("> O Stoping mqtt service.\n");
    int i =0;
    MQTTAsync_disconnectOptions disc_opts;
    disc_opts = bindDisconnectOptions(onDisconnect, onDisconnectFailure);
    for (i = 0; i < DEVICE_NUM; i++) {
        MQTTAsync_disconnect(mqttClientList[i], &disc_opts);
        printf("> O Number of %d mqtt client disconnect finished.\n", i);
        MQTTAsync_destroy(&mqttClientList[i]);
        printf("> O Number of %d mqtt client destroy finished.\n", i);
    }
    printf("> O Finished.\n\n");
    sleep(HALF_SECOND);
}