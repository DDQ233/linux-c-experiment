#include <stdio.h>
#include <unistd.h>
#include <MQTTAsync.h>
#include <string.h>

#define SERVER_IP "192.168.0.108"
#define PORT 1990
#define CLIENT_ID "1740707152"
#define USERNAME "admin"
#define PASSWORD "admin"

void onDisconnect(void* context, MQTTAsync_failureData* response)
{
    printf("> x Cannot mqtt server.\n");
    printf("> x Error : %d.\n", response->code);
}

void onSend(void* context, MQTTAsync_successData* data)
{

}

int main()
{
    int ret;
    MQTTAsync client;
    MQTTAsync_connectOptions conn_opt;
    int isConnect = 1;

    // Initialize mqtt client handle
    ret = MQTTAsync_create(&client, SERVER_IP, CLIENT_ID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    if (ret != MQTTASYNC_SUCCESS) {
        printf("> x Cannot create mqtt-async client.\n");
        printf("> x Error : %d.\n", ret);
        return -1;
    }

    conn_opt.username = USERNAME;
    conn_opt.password = PASSWORD;
    conn_opt.onFailure = onDisconnect;
    conn_opt.automaticReconnect = 1;
    ret = MQTTAsync_connect(client, &conn_opt);
    if (ret != MQTTASYNC_SUCCESS) {
        printf("> x Cannot start a connection to mqtt server.\n");
        printf("> x Error : %d.\n", ret);
        return -1;
    }

    while(1) {
        if(isConnect == 1) {
            MQTTAsync_message message = MQTTAsync_message_initializer;
            MQTTAsync_responseOptions resp_opt = MQTTAsync_responseOptions_initializer;
            char buffer[100];
            static int count = 1;
            sprintf(buffer, "number = %d", count++);
            message.payload = buffer;
            message.payloadlen = strlen(buffer);
            message.qos = 1;
            printf("> Sending message.......\n");
            ret = MQTTAsync_sendMessage(client, "Message", &message, &resp_opt);
            if (ret != MQTTASYNC_SUCCESS) {
                printf("> x Parameters error.\n");
            }
            printf("> Send successfully.\n");

        }
        sleep(2);
    }

    return 0;
}