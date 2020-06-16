#include <stdio.h>
#include <unistd.h>
#include <MQTTAsync.h>

#define SERVER_IP "127.0.0.1"
#define PORT 1990
#define CLIENT_ID "1740707152"
#define USERNAME "admin"
#define PASSWORD "admin"

int main()
{
    int ret;
    MQTTAsync client;
    MQTTAsync_connectOptions conn_opt;

    // Initialize mqtt client handle
    ret = MQTTAsync_create(&client, SERVER_IP, CLIENT_ID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    if (ret != MQTTASYNC_SUCCESS) {
        printf("> x Cannot create mqtt-async client.\n");
        return -1;
    }

    conn_opt.username = USERNAME;
    conn_opt.password = PASSWORD;
    ret = MQTTAsync_connect(client, &conn_opt);
    if (ret != MQTTASYNC_SUCCESS) {
        printf("> x Cannot start a connection to mqtt server.\n");
        return -1;
    }

    return 0;
}