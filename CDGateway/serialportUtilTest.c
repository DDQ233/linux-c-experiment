#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <MQTTAsync.h>
#include <cjson/cJSON.h>
#include "mqttutil.h"
#include "serialportutil.h"

#define MQTT_SERVER				"tcp://111.229.163.109:1990"
#define MQTT_CLIENTID			"TEST"
#define MQTT_USERNAME			"admin"
#define MQTT_PASSWORD			"admin"
#define MQTT_QOS				1
#define MQTT_PUBLISH_TOPIC		"/sensor"
#define MQTT_SUBSCRIBE_TOPIC	"/ctrl"

int isMqttConnected = 0;
int uartFd;

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
    char buffer[128];
    char data[4];
    char type[4];
    cJSON *json, *json1,*json_data, *json_type;
    sprintf(buffer, (char*)message->payload, strlen((char*)message->payload));
    printf("> Message --- > %s.\n", buffer);
    // Json data analysis
    json = cJSON_Parse(buffer);
    json1 = cJSON_Parse(buffer);
    json_data = cJSON_GetObjectItem(json, "data");
    sprintf(data, json_data->valuestring, strlen(json_data->valuestring));
    printf("> data : %s.\n", data);
    json_type = cJSON_GetObjectItem(json1, "type");
    sprintf(type, json_type->valuestring, strlen(json_type->valuestring));
    printf("> type : %s", type);

    if (strcmp(type, "ctrl") == 0) {
        printf("> CTRL.\n");
        if (strcmp(data, "O") == 0) {
            printf("> ON");
            write(uartFd, data, strlen(data));
        } else if (strcmp(data, "F") == 0) {
            printf("> OFF");
            write(uartFd, data, strlen(data));
        }
    }

	write(uartFd, (char*)message->payload, strlen((char*)message->payload));
	return 1;
}

int main()
{
	int count = 0;
	char receiveBuffer[128];
    // cJSON *json, *json_value;
	MQTTAsync mqttClient;
	MQTTAsync_connectOptions conn_opt = MQTTAsync_connectOptions_initializer;
    int ret;

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

    uartFd = UartOpen("/dev/ttyS1");
    uartFd = UartBindOptions(
        uartFd, 
        115200, 
        0, 
        8, 
        1, 
        'N');

    printf("> O Serialport initailize finished.\n");
    
    while(1) {
        /*
        write(uartFd, buffer, strlen(buffer));
        sleep(1);
        */
       /*
        printf("> Wait for data.\n\n");
        count = read(uartFd, receiveBuffer, 128);
        if (count > 0) {
            printf("> STR : %s\n", receiveBuffer);
            // Json data analysis
            json = cJSON_Parse(receiveBuffer);
            json_value = cJSON_GetObjectItem(json, "uid");
            printf("> uid : %s", json_value->valuestring);
            json_value = cJSON_GetObjectItem(json, "type");
            if (strcmp(json_value->valuestring, "ctrl") == 0) {
                json_value = cJSON_GetObjectItem(json, "D");
                if (strcmp(json_value->valuestring, "ON") == 0) {
                    write(uartFd, json_value->valuestring, strlen(json_value->valuestring));
                } else if (strcmp(json_value->valuestring, "OFF") == 0) {
                    write(uartFd, json_value->valuestring, strlen(json_value->valuestring));
                }
            }
            memset(receiveBuffer, 0, sizeof(receiveBuffer));
			count=0;
            */
           sleep(2);
        }
    close(uartFd);
    return 0;
}