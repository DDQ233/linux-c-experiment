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

#define MYSQL_REMOTE_SERVER_ADDRESS "111.229.163.109"
#define MYSQL_REMOTE_SERVER_USERNAME "ddq"
#define MYSQL_REMOTE_SERVER_PASSWORD "0619"
#define MYSQL_REMOTE_SERVER_DATABASE "iot-device"

#define MQTT_SERVER_ADDRESS "tcp://111.229.163.109:1990"
#define MQTT_CLIENT_ID "Gateway"
#define MQTT_USERNAME "admin"
#define MQTT_PASSWORD "admin"
#define MQTT_MESSAGE_QOS 1
#define MQTT_SENSOR_TOPIC_PERFIX "/sensor/"
#define MQTT_CONTORL_TOPIC "/ctrl/"

#define HALF_SECOND 0.5
#define ONE_SECOND 1
#define TWO_SECOND 2
#define FIVE_SECOND 5

static unsigned int isMqttConnected = 0;

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
    isMqttConnected = 1;
    MQTTAsync client = (MQTTAsync)context;
    // sleep(HALF_SECOND);
	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
    opts.onSuccess = onSubscribe;
	opts.onFailure = onSubscribeFailure;
    opts.context = client;
	int ret;
    if ((ret = MQTTAsync_subscribe(client, MQTT_CONTORL_TOPIC, MQTT_MESSAGE_QOS, NULL)) != MQTTASYNC_SUCCESS) {
        printf("> x Failed to subscribe topic.\n");
        printf("> x Error code : %d.\n", ret);
    } else {
        printf("> O Subscribe topic finished.\n");
    }
}

void onConnectFailure(void* context, MQTTAsync_failureData* response)
{
    printf("> x Failed to connect mqtt server.\n");
}

void connlost(void *context, char *cause)
{
    printf("> x Connect lost.\n");
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

void onDisconnectFailure(void* context, MQTTAsync_failureData* response)
{
    printf("> x Failed to disconnect mqtt server.\n");
}

void onDisconnect(void* context, MQTTAsync_successData* response)
{
    printf("> O Disconnect mqtt server successfully.\n");
}

void onSendFailure(void* context, MQTTAsync_failureData* response)
{
    printf("> x Failed to send message.\n");
}

void onSend(void* context, MQTTAsync_successData* response)
{
    printf("> O Send message successfully.\n");
}


int main()
{
    int ret;
    int uartFd;
    int count;
    char sql[128];
    char receiveBuffer[128];
    char temp[128];
    struct termios uart_cfg;
    MQTTAsync mqttClient;
    MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
    MYSQL com_mysql;
    sleep(ONE_SECOND);

    // Initialize serialport
    printf("> O Initializing serial port.\n");
    // Open serialport
    uartFd = open("/dev/ttyS0", O_RDWR|O_NONBLOCK|O_NOCTTY);
    if (uartFd < 0) {
		perror("> x Cannot open serial port.\n");
		return -1;
	}
    printf("> O Open serialport 1.\n");
    // Fcntl
    if (fcntl(uartFd, F_SETFL, 0) < 0) {
		perror("> x Failed to fcntl.\n");
		return -1;
	}
    // Set uart type as original pattern
    cfmakeraw(&uart_cfg);
    printf("> O Set uart type as original pattern.\n");
    // Activate local connection
	uart_cfg.c_cflag |= CLOCAL;
    printf("> O Activate local connection.\n");
    // Recevicing enabled
	uart_cfg.c_cflag |= CREAD;
    printf("> O Activate local connection and recevicing enabled.\n");
    // Set baudrate
    cfsetspeed(&uart_cfg, B115200);
    printf("> O Set baudrate.\n");
    // Set flow control
    uart_cfg.c_cflag &= ~CRTSCTS;
    printf("> O Set flow control.\n");
    // Set data bit
    uart_cfg.c_cflag |= CS8;
    printf("> O Set data bit.\n");

    // Set parity check
    printf("> O Set sopt bit.\n");

    // Set sopt bit
    uart_cfg.c_cflag &= ~CSTOPB;

    // Clear buffer zone
	tcflush(uartFd, TCIOFLUSH);
    printf("> O Clear buffer zone.\n");

    // Activate uart settings
    if (tcsetattr(uartFd, TCSANOW, &uart_cfg) != 0) {
		perror("> x Failed to activate uart settings.\n");
		return -1;
	}
	printf("> O Activate uart settings.\n");

    printf("> O Finished.\n\n");

    sleep(ONE_SECOND);

    /******************************************************************/

    printf("> O Initializing mqtt client.\n");
    // Create mqtt client.
    ret = MQTTAsync_create(
        &mqttClient, 
        MQTT_SERVER_ADDRESS, 
        MQTT_CLIENT_ID,
        MQTTCLIENT_PERSISTENCE_NONE,
        NULL);
    if (ret != MQTTASYNC_SUCCESS) {
        printf("> x Failed to create mqtt client.\n");
        printf("> x Error code : %d.\n", ret);
        return -1;
    }
    printf("> O Create mqtt client.\n");

    // Set callback function.
    ret = MQTTAsync_setCallbacks(mqttClient, NULL, NULL, messageArrived, NULL);
    if (ret != MQTTASYNC_SUCCESS) {
        printf("> x Cannot set callbacks function.\n");
        printf("> x Error code : %d.\n", ret);
        return -1;
	}
    printf("> O Set callbacks function.\n");

    // Bind connect options
    conn_opts.username = MQTT_USERNAME;
    conn_opts.password = MQTT_PASSWORD;
    conn_opts.onSuccess = onConnect;
    conn_opts.onFailure = onConnectFailure;
    conn_opts.context = mqttClient;
    printf("> O Bind connect options.\n");

    // Connect mqtt server
    if (ret != MQTTASYNC_SUCCESS) {
        printf("> x Cannot connect mqtt server.\n");
        printf("> x Error code : %d.\n", ret);
        return -1;
	}
    printf("> O Connect mqtt server.\n");

    printf("> O Finshed.\n\n");

    sleep(ONE_SECOND);

    /******************************************************************/

    printf("> O Initializing mysql client.\n");

    // Initialize mysql client
    if (mysql_init(&com_mysql) == NULL) {
        printf("> x Cannot init mysql.\n");
        return -1;
    }
    printf("> O Initialize mysql client.\n");

    // Connect mysql server.
    if(mysql_real_connect(
            &com_mysql, 
            MYSQL_REMOTE_SERVER_ADDRESS, 
            MYSQL_REMOTE_SERVER_USERNAME, 
            MYSQL_REMOTE_SERVER_PASSWORD, 
            MYSQL_REMOTE_SERVER_DATABASE, 
            0, NULL, 0) == NULL) {
        printf("> x %s\n", mysql_error(&com_mysql));
        return -1;
    }
    printf("> O Connect msyql server.\n");

    printf("> O Finished.\n\n");

    sleep(ONE_SECOND);

    /******************************************************************/

    printf("> O Application running.........");

    // Run
    while(1) {
        if (isMqttConnected == 1) {
            // Read from serialport
            count = read(uartFd, receiveBuffer, 128);
            
            if(count > 0 ) {
                printf("> Serialport msg ---> %s.\n", receiveBuffer);

                MQTTAsync_message message = MQTTAsync_message_initializer;
                message.payload = receiveBuffer;
                message.payloadlen = (int)strlen(receiveBuffer);
                message.qos = MQTT_MESSAGE_QOS;
                // Wait message sended
                while((ret = MQTTAsync_sendMessage(mqttClient, "/sensor/123", &message, NULL)) != MQTTASYNC_SUCCESS) {
                    sleep(HALF_SECOND);
                }
                memset(receiveBuffer, 0, sizeof(receiveBuffer));
                count = 0;
            }
        }
    }

    close(uartFd);
    // mysql_close(&com_mysql);
    return 0;
}

