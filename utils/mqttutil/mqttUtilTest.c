#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <MQTTAsync.h>
#include "mqttutil.h"

#define SERVER_ADDRESS "tcp://111.229.163.109:1990"
#define CLIENT_ID "Gateway"
#define USERNAME "admin"
#define PASSWORD "admin"
#define QOS 1
#define SENSOR_TOPIC_PERFIX "/sensor/"
#define CONTORL_TOPIC_PERFIX "/ctrl/"
#define KEEP_ALIVE_INTERVAL_TIME 20
#define FLAG_CLEAN_SESSION 0
#define FLAG_AUTOMATIC_RECONNECT 1

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
    // Variables
    MQTTAsync mqttClient;
    MQTTAsync_connectOptions conn_opts;
    MQTTAsync_responseOptions resp_opts;
    MQTTAsync_disconnectOptions disc_opts;
    int ret;

    // Bind connect options
    conn_opts = bindConnectOptions(
        USERNAME, 
        PASSWORD, 
        KEEP_ALIVE_INTERVAL_TIME, 
        FLAG_CLEAN_SESSION, 
        FLAG_AUTOMATIC_RECONNECT, 
        onConnect, 
        onConnectFailure);

    // Bind resoponse Options
    resp_opts = bindResponseOptions(onSend, onSendFailure);

    // Bind disconnect options
    disc_opts = bindDisconnectOptions(onDisconnect, onDisconnectFailure);

    // Create mqtt client handle.
    mqttClient = createClient(SERVER_ADDRESS, CLIENT_ID);

    // Set callback function.
    mqttClient = setCallbacks(mqttClient, connlost, messageArrived, NULL);

    // Connect mqtt server
    mqttClient = connectMqttServer(mqttClient, conn_opts);

    while(1) {
        MQTTAsync_message message = MQTTAsync_message_initializer;
        char buffer[100];
        sprintf(buffer, "{T:233,H:233}");
        message.payload = buffer;
        message.payloadlen = (int)strlen(buffer);
        message.qos = QOS;
        printf("> Sending message.......\n");
        if ((ret = MQTTAsync_sendMessage(mqttClient, "/sensor/dht11", &message, NULL)) != MQTTASYNC_SUCCESS) {
            printf("> x Parameters error.\n");
        } else {
            printf("> Send successfully.\n");
        }
        sleep(2);
    }

    MQTTAsync_destroy(&mqttClient);

}