#include <MQTTAsync.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>
#include <mysql/mysql.h>

#define SERIAL_PORT				"/dev/ttyS0"

#define MQTT_SERVER				"tcp://111.229.163.109:1990"
#define MQTT_CLIENTID			"Gateway"
#define MQTT_USERNAME			"admin"
#define MQTT_PASSWORD			"admin"
#define MQTT_QOS				1
#define MQTT_PUBLISH_TOPIC		"/sensor/"
#define MQTT_SUBSCRIBE_TOPIC	"/ctrl/"

#define MYSQL_SERVER			"111.229.163.109"
#define MYSQL_USERNAME			"yuki"
#define MYSQL_PASSWORD			"0619"
#define MYSQL_DATABASE			"iot-device"

#define HALF_SECOND 0.5
#define ONE_SECOND 1
#define TWO_SECOND 2
#define FIVE_SECOND 5

#define TOPIC1 "/sensor"
#define TOPIC2 "/ctrl"

static int isMqttConnected = 0;
static int uartFd;

int temp;
int hum;

void onSubscirbe(void* context, MQTTAsync_successData* response)
{
    printf("> O Subscribe succeeded\n");
}
void onConnect(void *context,MQTTAsync_successData* response)
{
	printf("> O Connect mqtt server successfully.\n");
	int ret;
	MQTTAsync_responseOptions response_opt = MQTTAsync_responseOptions_initializer;
	MQTTAsync mqttClient = (MQTTAsync)context;
	isMqttConnected = 1;
	response_opt.onSuccess = onSubscirbe;
	ret = MQTTAsync_subscribe(mqttClient, MQTT_SUBSCRIBE_TOPIC, MQTT_QOS, &response_opt);
	if (ret != MQTTASYNC_SUCCESS) {
		printf("> x Failed to subscribe topic.\n");
        printf("> x Error code : %d.\n", ret);	
	} else {
		printf("> O Subscribe topic finished.\n");
	}
}

void onDisconnect(void *context,MQTTAsync_failureData* response)
{
	printf("> x Failed to connect mqtt server.\n");
}

int messageArrived(void* context,char* topicName,int topiclen,MQTTAsync_message* message)
{
    printf("> Message --- >%s.\n",(char*)message->payload);
	write(uartFd, (char*)message->payload, strlen((char*)message->payload));
	return 1;
}

int main()
{
	
	int count=0;
	char buffer[64];
	char buffer2[64];
	char buf[20];
	MYSQL com_mysql;
	char sqlcommand[128];
	time_t now;
	struct tm *tm_now;
	struct termios uart_cfg;
	MQTTAsync mqttClient;
	MQTTAsync_connectOptions conn_opt = MQTTAsync_connectOptions_initializer;
	int ret;

	uartFd=open(SERIAL_PORT, O_RDWR|O_NONBLOCK|O_NOCTTY);
	if (uartFd < 0) {
		perror("> x Cannot open serial port.\n");
		return -1;
	}
    fcntl(uartFd,F_SETFL,0);
	cfmakeraw(&uart_cfg);
	cfsetspeed(&uart_cfg,B9600);	
    uart_cfg.c_cflag|=CLOCAL|CREAD;
	tcflush(uartFd,TCIOFLUSH);
	tcsetattr(uartFd,TCSANOW,&uart_cfg);

	printf("> O Serial port initailize finished.\n");

	ret = MQTTAsync_create(&mqttClient, MQTT_SERVER, MQTT_CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
	if (ret != MQTTASYNC_SUCCESS) {
        printf("> x Failed to create mqtt client.\n");
        printf("> x Error code : %d.\n", ret);
        return -1;
    }
	
	ret = MQTTAsync_setCallbacks(mqttClient, NULL, NULL, messageArrived, NULL);
	if (ret != MQTTASYNC_SUCCESS) {
        printf("> x Cannot set callbacks function.\n");
        printf("> x Error code : %d.\n", ret);
        return -1;
	}

	conn_opt.username = MQTT_USERNAME;
	conn_opt.password = MQTT_PASSWORD;
	conn_opt.onSuccess = onConnect;
	conn_opt.onFailure = onDisconnect;
	conn_opt.context = mqttClient;
	
	ret = MQTTAsync_connect(mqttClient, &conn_opt);
	if (ret != MQTTASYNC_SUCCESS) {
        printf("> x Cannot connect mqtt server.\n");
        printf("> x Error code : %d.\n", ret);
        return -1;
	}

	printf("> O Mqtt client initailize finished.\n");
	
	if (mysql_init(&com_mysql) == NULL) {
		printf("> x Cannot initialize mysql.\n");
        return -1;
    }
	if (mysql_real_connect(&com_mysql, MYSQL_SERVER, MYSQL_USERNAME , MQTT_PASSWORD, MYSQL_DATABASE, 0, NULL, 0) == NULL) {
		printf("> x Cannot connect mysql server.\n");
		printf("> x Error msg : %s\n", mysql_error(&com_mysql));
		return -1;
	}

	// printf("> O Mysql client initailize finished.\n");

	printf("\n\n> Application running.............\n");

	while(1)
	{
		/*
		if(isMqttConnected == 1)
		{
			count=read(uartFd,buffer,64);
        		if(count>0)
        		{
				time(&now);
				tm_now = localtime(&now);
				sprintf(buf,"%d-%d-%d %d:%d:%d",tm_now->tm_year+1900, tm_now->tm_mon+1, tm_now->tm_mday, tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec);
				printf("%s\n",buf);
				sscanf(buffer,"%d %d",&temp,&hum);
            			printf("temp:%d,hum:%d\r\n\n",temp,hum);
				MQTTAsync_message message = MQTTAsync_message_initializer;
				static int count = 1;
				sprintf(buffer2,"temp:%d,hum:%d",temp,hum);
				message.payload = buffer2;
				message.payloadlen = strlen(buffer2);
				message.qos=1;
				ret = MQTTAsync_sendMessage(mqttClient,TOPIC1,&message,NULL);
				if(ret != MQTTASYNC_SUCCESS)
				{
					printf("参数错误！\n");
				}
				sprintf(sqlcommand,"insert into keshe_kesheinfo(date,temp,Hum) values('%s',%d,%d)",buf,temp,hum);
				if(mysql_query(&com_mysql,sqlcommand) != 0)
				{
					printf("%s\n",mysql_error(&com_mysql));	
				}
        		}
		}
		*/
		printf("> while.\n");
		sleep(ONE_SECOND);
	}

	close(uartFd);
	mysql_close(&com_mysql);
	return 0;
}
