#include <MQTTAsync.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>
#include <mysql/mysql.h>

#define MQTT_SERVER				"tcp://111.229.163.109:1990"
#define MQTT_CLIENTID			"Gateway"
#define MQTT_USERNAME			"admin"
#define MQTT_PASSWORD			"admin"
#define MQTT_QOS				1
#define MQTT_PUBLISH_TOPIC		"/sensor/"
#define MQTT_SUBSCRIBE_TOPIC	"/ctrl/"

#define MYSQL_SERVER			"111.229.163.109"
#define MYSQL_USERNAME			"ddq"
#define MYSQL_PASSWORD			"0619"
#define MYSQL_DATABASE			"iot-device"

static int isConnected = 0;
static int uartFd;
int temp;
int hum;

void onSubscirbe(void* context, MQTTAsync_successData* response)
{
    printf("> O Subscribe succeeded\n");
}
void onConnect(void *context,MQTTAsync_successData* response)
{
	int ret;
	MQTTAsync_responseOptions response_opt = MQTTAsync_responseOptions_initializer;
	MQTTAsync client = (MQTTAsync)context;
	printf("> O Connect mqtt server successfully.\n");
	isConnected = 1;
	response_opt.onSuccess = onSubscirbe;
	ret = MQTTAsync_subscribe(client, MQTT_SUBSCRIBE_TOPIC, 1, &response_opt);
	if(ret != MQTTASYNC_SUCCESS) {
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

int msgarrvd(void* context,char* topicName,int topiclen,MQTTAsync_message* message)
{
    printf("> Msg ---> %s\n", (char*)message->payload);
	write(uartFd, (char*)message->payload, strlen((char*)message->payload));
	return 1;
}

int main()
{
	
	int count=0;
	char receiveBuffer[128];
	char buffer2[64];
	char buf[20];
	char timestamp[64];
	MYSQL mysqlClient;
	char sql[256];
	time_t t;
	struct tm *tm;
	struct termios uart_cfg;
	MQTTAsync mqttClient;
	MQTTAsync_connectOptions conn_opt = MQTTAsync_connectOptions_initializer;
	int ret;

	printf("> O Initializing........\n");

	uartFd = open("/dev/ttyS1", O_RDWR|O_NONBLOCK|O_NOCTTY);
	if(uartFd<0)
	{
		perror("> x Failed to open serial port.\n");
		return -1;
	}
    fcntl(uartFd, F_SETFL, 0);
	cfmakeraw(&uart_cfg);
	cfsetspeed(&uart_cfg, B115200);
    uart_cfg.c_cflag|=CLOCAL|CREAD;
	tcflush(uartFd, TCIOFLUSH);
	tcsetattr(uartFd, TCSANOW, &uart_cfg);

	printf("> O Serial port initailize finished.\n");

	ret = MQTTAsync_create(&mqttClient, MQTT_SERVER, MQTT_CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
	if (ret != MQTTASYNC_SUCCESS) {
        printf("> x Failed to create mqtt client.\n");
        printf("> x Error code : %d.\n", ret);
        return -1;
    }
	
	ret = MQTTAsync_setCallbacks(mqttClient, NULL, NULL, msgarrvd, NULL);
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
	
	printf("> O Mqtt initialize finished.\n");

	if (mysql_init(&mysqlClient) == NULL) {
		printf("> x Cannot initialize mysql.\n");
        return -1;
    }
	if(mysql_real_connect(&mysqlClient, MYSQL_SERVER, MYSQL_USERNAME, MYSQL_PASSWORD, MYSQL_DATABASE, 0, NULL, 0) == NULL)
	{
		printf("> x Cannot connect mysql server.\n");
		printf("> x Error msg : %s\n", mysql_error(&mysqlClient));
		return -1;
	}
	printf("> O Mysql client initialized.\n");
	printf("> O Application running...");

	while(1)
	{
		if(isConnected == 1) {
			count = read(uartFd, receiveBuffer, 128);
			if(count>0) {
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
				if (mysql_query(&mysqlClient, sql) != 0) {
					printf("> x Excute sql error : %s.\n\n", mysql_error(&mysqlClient));
				} else {
					printf("> O Excute sql finished.\n");
				}
				MQTTAsync_message message = MQTTAsync_message_initializer;
				message.payload = receiveBuffer;
				message.payloadlen = (int)strlen(receiveBuffer);
				message.qos = MQTT_QOS;
				while((ret = MQTTAsync_sendMessage(mqttClient, "/sensor/123", &message, NULL)) != MQTTASYNC_SUCCESS){
					sleep(0.5);
				}
			}
				/*
				time(&t);
				tm = localtime(&t);
				sprintf(buf,"%d-%d-%d %d:%d:%d",tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
				printf("%s\n",buf);
				sscanf(buffer,"%d %d",&temp,&hum);
            			printf("temp:%d,hum:%d\r\n\n",temp,hum);
				MQTTAsync_message message = MQTTAsync_message_initializer;
				static int count = 1;
				sprintf(buffer2,"temp:%d,hum:%d",temp,hum);
				message.payload = buffer2;
				message.payloadlen = strlen(buffer2);
				message.qos=1;
				ret = MQTTAsync_sendMessage(client,TOPIC1,&message,NULL);
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
				*/
		}
	}

	close(uartFd);
	mysql_close(&mysqlClient);
	return 0;
}
