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

static unsigned int isConnect = 0;
static unsigned int isFinshed = 0;
static unsigned int count = 0;

void onConnect(void* context, MQTTAsync_successData* response)
{
    MQTTAsync client = (MQTTAsync)context;
    int ret;
    ret = MQTTAsync_subscribe(client, "/hello", QOS, NULL);
    if (ret != MQTTASYNC_SUCCESS) {
        printf("> x Cannot subscribe topic.\n");
    } else {
        printf("> O Subscribe topic successfully.\n");
    }
    isConnect = 1;
}

void onDisconnect(void* context, MQTTAsync_failureData* response)
{
    int ret;
    MQTTAsync client = (MQTTAsync)context;
    printf("> x Cannot connect mqtt server.\n");
    printf("> x Error : %d.\n", response->code);
    // Connect mqtt server
    // if ((ret = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS) {
    //     printf("> x Cannot start a connection to mqtt server.\n");
    //     printf("> x Error : %d.\n", ret);
    //     isConnect = 0;
    //     return -1;
    // } else {
    //     isConnect = 1;
    //     printf("> O Connect mqtt server successfully.\n");
    // }
}

int onMessageArrived(void* context, char* topicName, int topicLen, MQTTAsync_message* message)
{
    printf("Payload : %s.\n", (char*)message->payload);
    MQTTAsync_free(topicName);
    MQTTAsync_free(message);
    return 1;
}

int main()
{
    // Variables
    MQTTAsync client;
    MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
    int ret;

    // Create mqtt client handle
    if ((ret = MQTTAsync_create(&client, SERVER_IP, CLIENT_ID, MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTASYNC_SUCCESS) {
        printf("> x Cannot create mqtt-async client.\n");
        printf("> x Error : %d.\n", ret);
        isConnect = 0;
        return -1;
    } else {
        isConnect = 1;
        printf("> O Create handle successfully.\n");
    }

    // Set callback function
    if ((ret = MQTTAsync_setCallbacks(client, NULL, NULL, onMessageArrived, NULL)) != MQTTASYNC_SUCCESS) {
        printf("> x Cannot set callback function.\n");
        return -1;
    } else {
        isConnect = 1;
        printf("> O Set callback function successfully.\n");
    }

    // Bind connect options
    conn_opts.username = USERNAME;
    conn_opts.password = PASSWORD;
    conn_opts.onSuccess = onConnect;
    conn_opts.onFailure = onDisconnect;
    conn_opts.automaticReconnect = 1;
    conn_opts.context=client;
    conn_opts.cleansession=0;
    conn_opts.context = client;
    // Connect mqtt server
    if ((ret = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS) {
        printf("> x Cannot start a connection to mqtt server.\n");
        printf("> x Error : %d.\n", ret);
        isConnect = 0;
        return -1;
    } else {
        isConnect = 1;
        printf("> O Connect mqtt server successfully.\n");
    }

    while(1) {
        if(isConnect == 1) {
            MQTTAsync_message message = MQTTAsync_message_initializer;
            MQTTAsync_responseOptions resp_opts = MQTTAsync_responseOptions_initializer;
            char buffer[100];
            sprintf(buffer, "{T:233,H:233}");
            message.payload = buffer;
            message.payloadlen = (int)strlen(buffer);
            message.qos = QOS;
            printf("> Sending message.......\n");
            if ((ret = MQTTAsync_sendMessage(client, TOPIC, &message, &resp_opts)) != MQTTASYNC_SUCCESS) {
                printf("> x Parameters error.\n");
            } else {
                printf("> Send successfully.\n");
            }
        }
        sleep(3);
    }
    return 0;
}