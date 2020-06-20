#include <stdio.h>
#include <unistd.h>
#include <MQTTAsync.h>
#include <string.h>

#define SERVER_IP "tcp://111.229.163.109:1990"
#define PORT 1990
#define CLIENT_ID "1740707152"
#define USERNAME "admin"
#define PASSWORD "admin"
#define QOS 1
#define TOPIC "/sensor/dht11"

int isConnect = 0;

void onDisconnect(void* context, MQTTAsync_failureData* response)
{
    printf("> x Cannot mqtt server.\n");
    printf("> x Error : %d.\n", response->code);
}

void onSend(void* context, MQTTAsync_successData* data)
{
    printf("> O Send successfully.\n");
}

int onMessageArrived(void* context, char* topicName, int topicLen, MQTTAsync_message* message)
{
    printf("Payload : %s.\n", (char*)message->payload);
    MQTTAsync_free(topicName);
    MQTTAsync_free(message);
    return 1;
}

void onSubscribe(void* context, MQTTAsync_successData* response)
{
    printf("> O Subscribe topic successfully.\n");
}

void onConnect(void* context, MQTTAsync_successData* response)
{
    int ret;
    MQTTAsync client = (MQTTAsync) context;
    MQTTAsync_responseOptions resp_opt = MQTTAsync_responseOptions_initializer;
    printf("> O Connected with mqtt server.\n");
    resp_opt.onSuccess = onSubscribe;
    ret = MQTTAsync_subscribe(client, "Message", 1, &resp_opt);
    if (ret != MQTTASYNC_SUCCESS) {
        printf("> x Parameters error.\n");
    }
}

int main()
{
    int ret;
    MQTTAsync client;
    MQTTAsync_connectOptions conn_opt = MQTTAsync_connectOptions_initializer;

    // Initialize mqtt client handle
    ret = MQTTAsync_create(&client, SERVER_IP, CLIENT_ID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    if (ret != MQTTASYNC_SUCCESS) {
        printf("> x Cannot create mqtt-async client.\n");
        printf("> x Error : %d.\n", ret);
        return -1;
    }

    // Set callback function
    ret = MQTTAsync_setCallbacks(client, NULL, NULL, onMessageArrived, NULL);
    if (ret != MQTTASYNC_SUCCESS) {
        printf("> x Cannot set callback function.\n");
        return -1;
    }

    conn_opt.username = USERNAME;
    conn_opt.password = PASSWORD;
    conn_opt.onFailure = onDisconnect;
    conn_opt.automaticReconnect = 1;
    conn_opt.context=client;
    conn_opt.cleansession=0;
    ret = MQTTAsync_connect(client, &conn_opt);
    if (ret != MQTTASYNC_SUCCESS) {
        printf("> x Cannot start a connection to mqtt server.\n");
        printf("> x Error : %d.\n", ret);
        return -1;
    }

    isConnect = 1;

    while(1) {
        if(isConnect == 1) {
            MQTTAsync_message message = MQTTAsync_message_initializer;
            // MQTTAsync_responseOptions resp_opt = MQTTAsync_responseOptions_initializer;
            char buffer[100];
            static int count = 1;
            sprintf(buffer, "number = %d", count++);
            message.payload = buffer;
            message.payloadlen = strlen(buffer);
            message.qos = 1;
            printf("> Sending message.......\n");
            ret = MQTTAsync_sendMessage(client, TOPIC, &message, NULL);
            if (ret != MQTTASYNC_SUCCESS) {
                printf("> x Parameters error.\n");
            }
            printf("> Send successfully.\n");

        }
        sleep(2);
    }

    return 0;
}