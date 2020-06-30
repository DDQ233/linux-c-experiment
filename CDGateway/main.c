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
#define MQTT_CONTORL_TOPIC "/ctrl/"
#define MQTT_KEEP_ALIVE_INTERVAL_TIME 30
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

#define DEVICE_NUM 2
// #define UART_FD_PREFIX "/dev/ttyS"

// static unsigned int isMysqlConnect = 0;
// static unsigned int isMqttConnect = 0;
// static unsigned int isAllSerialportConnect = 0;
static unsigned int deviceStatus[DEVICE_NUM];
static char sensorMqttTopic[DEVICE_NUM][20];
static int uartFdPool[DEVICE_NUM];
static pthread_t serialportThreadPool[DEVICE_NUM];
static MYSQL mysqlClientPool[DEVICE_NUM];
static MYSQL_RES *pRes;
static MQTTAsync mqttClientList[DEVICE_NUM];
// static char* pSerialport;
// static char serialportList[DEVICE_NUM][15];
// static char* serialport[DEVICE_NUM];
// static MYSQL mysqlClient;
// static MQTTAsync mqttclient;

void onSubscribe(void* context, MQTTAsync_successData* response)
{
	printf("> O Subscribe succeeded\n");
}

void onSubscribeFailure(void* context, MQTTAsync_failureData* response)
{
	printf("> x Subscribe failed, rc %d\n", response->code);
}

void onConnect(void* context, MQTTAsync_successData* response)
{
    printf("> O Connect mqtt server successfully.\n");
    MQTTAsync client = (MQTTAsync)context;
    // sleep(HALF_SECOND);
    client = subscribeTopic(client, MQTT_CONTORL_TOPIC, MQTT_MESSAGE_QOS);
	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
    opts.onSuccess = onSubscribe;
	opts.onFailure = onSubscribeFailure;
    opts.context = client;
	int ret;
    // client = subscribeTopic(client, MQTT_CONTORL_TOPIC, MQTT_MESSAGE_QOS, opts);
    if ((ret = MQTTAsync_subscribe(client, MQTT_CONTORL_TOPIC, MQTT_MESSAGE_QOS, NULL)) != MQTTASYNC_SUCCESS) {
        printf("> x Failed to subscribe topic.\n");
        printf("> x Error code : %d.\n", ret);
    } else {
        printf("> O Subscribe topic finished.\n");
    }
}

void onDisconnect(void* context, MQTTAsync_successData* response)
{
    printf("> O Disconnect mqtt server successfully.\n");
}

int messageArrived(void* context, char* topicName, int topicLen, MQTTAsync_message* message)
{
    printf("> Message arrived.\n");
    printf("> Topic : %s.\n", topicName);
    printf("> Message : %s.\n", (char*)message->payload);
    MQTTAsync_freeMessage(&message);
    MQTTAsync_free(topicName);
    return 1;
}

void connlost(void *context, char *cause)
{
    /*
    MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
    int ret;

    printf("\n> x Connection lost.\n");
    printf("> x Cause : %s.\n", cause);
    printf("> O Reconnecting..........\n");
    conn_opts.keepAliveInterval = MQTT_KEEP_ALIVE_INTERVAL_TIME;
    conn_opts.cleansession = MQTT_FLAG_CLEAN_SESSION;
	if ((ret = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS) {
        printf("> x Failed to reconnect mqtt server.");
        printf("> x Error code : %d", ret);
    } else {
        printf("> O Reconnect finished.\n\n");
    }
    */
}

void onConnectFailure(void* context, MQTTAsync_failureData* response)
{
    printf("> x Failed to connect mqtt server.\n");
}

void onDisconnectFailure(void* context, MQTTAsync_failureData* response)
{
    printf("> x Failed to disconnect mqtt server.\n");
}

void onSendFailure(void* context, MQTTAsync_failureData* response)
{
    // printf("> x Failed to send message.\n");
}

void onSend(void* context, MQTTAsync_successData* response)
{
    printf("> O Send message successfully.\n");
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
    initSerialportList();
    startMySqlService();
    startMqttService();
    startSerialportThread();

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

    printf("> O Application running........\n\n");

    // closeSerialportThread();
    // closeSerialport();
    // stopMysqlService();
    // stopMqttService();

    while(1) {
        sleep(TWO_SECOND);
    }

    return 0;
}

void *serialportThread(void *arg)
{
    // Variables
    time_t t;
    struct tm *tm;
    char timestamp[64];
    char receiveBuffer[128];
    int j = 0;
    int deviceNum = *((int*)arg);
    int count = 0;
    int ret;
    int fd = uartFdPool[deviceNum];
    cJSON *json, *json_value;
    char topic[20];
    int flag = -1;
    char sql[256];
    // MQTTAsync client = mqttClientList[deviceNum];
    // Subscribe topic
    MQTTAsync_message message = MQTTAsync_message_initializer;
    MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
    opts.onSuccess = onSend;
	opts.onFailure = onSendFailure;
	opts.context = mqttClientList[deviceNum];
    printf("> O Thread %d running.\n", deviceNum);

    // Run
    while(1) {
        count = read(uartFdPool[deviceNum], receiveBuffer, 128);
        if (count > 0) {

            // // Json data analysis
            // json = cJSON_Parse(receiveBuffer);
            // json_value = cJSON_GetObjectItem(json, "uid");
            // // Dynamic binding topic
            // if (strlen(sensorMqttTopic[deviceNum]) == 0) {
            //     sprintf(sensorMqttTopic[deviceNum], MQTT_SENSOR_TOPIC_PERFIX);
            //     strcat(sensorMqttTopic[deviceNum], json_value->valuestring);
            //     for (j = 0; j < strlen(sensorMqttTopic[deviceNum]); j++) {
            //         topic[j] = sensorMqttTopic[deviceNum][j];mai
            //     }
            // } else if (strlen(sensorMqttTopic[deviceNum]) != 0) {
            //     // printf("> Topic -----> %s.\n", sensorMqttTopic[deviceNum]);
            // } else {
            //     printf("> x Failed to bind topic.\n");
            // }

            printf("> Msg ---> %s.\n", receiveBuffer);

            // Get local time for now as timestamp
            time(&t);
            tm = localtime(&t);
            sprintf(
                timestamp, 
                "%4d-%02d-%02d %02d:%02d:%02d", 
                tm->tm_year + 1900,
                tm->tm_mon + 1,
                tm->tm_mday,
                tm->tm_hour,
                tm->tm_min,
                tm->tm_sec);
            sprintf(sql, "INSERT INTO tb_data VALUES('user1', '00000001', '%s', '%s')", receiveBuffer, timestamp);
            if (mysql_query(&mysqlClientPool[deviceNum], sql) != 0) {
                printf("> x Excute sql error : %s.\n\n", mysql_error(&mysqlClientPool[deviceNum]));
            } else {
                printf("> O Excute sql finished.\n");
            }

            // Message binding
            message.payload = receiveBuffer;
            message.payloadlen = (int)strlen(receiveBuffer);
            message.qos = MQTT_MESSAGE_QOS;
            while((ret = MQTTAsync_sendMessage(mqttClientList[deviceNum], "/sensor/123", &message, &opts)) != MQTTASYNC_SUCCESS){
                sleep(HALF_SECOND);
            }
            /*
            // if ((ret = MQTTAsync_sendMessage(mqttClientList[deviceNum], topic, &message, &opts)) != MQTTASYNC_SUCCESS) {
            //     // printf("> x Failed to send message. Error code : %d\n", ret);
            // } else {
            //     // printf("> Send -----> %s.\n", receiveBuffer);
            // }
            */
            
            // Clear receiver buffer
            memset(receiveBuffer, 0, sizeof(receiveBuffer));
            count = 0;
        }
    }
}

void initDeviceStatus()
{
    printf("> O Initializing all device status.\n");
    int i = 0;
    for (i = 0; i < DEVICE_NUM; i++) {
        deviceStatus[i] = 0;
    }
    printf("> O Finished.\n\n");
    sleep(ONE_SECOND);
}

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
    sleep(ONE_SECOND);
}

void startSerialportThread()
{
    printf("> O Starting serial port thread.\n");
    int j = 0;
    int ret;
    for (j = 0; j < DEVICE_NUM; j++) {
        ret = pthread_create(&serialportThreadPool[j], NULL, serialportThread, (void*)&j);
        if (ret < 0) {
            printf("> x Failed to create number of %d pthread.\n", j);
        } else {
            printf("> O Number of %d thread was created.\n\n", j);
        }
        sleep(ONE_SECOND);
    }
    printf("> O Finished.\n\n");
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
    int i = 0;
    for (i = 0; i < DEVICE_NUM; i++) {
        mysqlClientPool[i] = connectMysql(
            MYSQL_REMOTE_SERVER_ADDRESS, 
            MYSQL_REMOTE_SERVER_USERNAME, 
            MYSQL_REMOTE_SERVER_PASSWORD, 
            MYSQL_REMOTE_SERVER_DATABASE);
        printf("> O Mysql service %d was started.\n", i);
    }
    sleep(ONE_SECOND);
    printf("> O Finished.\n\n");
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
        conn_opts.context = mqttClientList[i];
        mqttClientList[i] = createClient(MQTT_SERVER_ADDRESS, MQTT_CLIENT_ID);
        mqttClientList[i] = setCallbacks(mqttClientList[i], connlost, messageArrived, NULL);
        mqttClientList[i] = connectMqttServer(mqttClientList[i], conn_opts);
        // mqttClientList[i] = subscribeTopic(mqttClientList[i], MQTT_CONTORL_TOPIC, MQTT_MESSAGE_QOS);
        printf("> O Number of %d mqtt client initialized.\n\n", i);
        sleep(ONE_SECOND);
    }
    printf("> O Mqtt service was started.\n\n");
    sleep(ONE_SECOND);
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
    int i = 0;
    for (int i = 0; i < DEVICE_NUM; i++) {
        mysql_close(&mysqlClientPool[i]);
    }
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
