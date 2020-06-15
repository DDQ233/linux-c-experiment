#include <MQTTAsync.h>
#include <stdio.h>
#include <unistd.h>

#define SERVER_ADDRESS "tcp://192.168.0.108:1990"
#define CLIENT_ID "152"
#define USERNAME "admins"
#define PASSWORD "admins"

void onConnect(void *context, MQTTAsync_successData* response)
{
    printf("> O Succeed in connecting to mqtt server.");
}

/**
 * 1. declare mqtt client handle
 * 2. create mqtt client 
 * 3. connect to mqtt server
 */
int main()
{
    // MQTT client handle
    MQTTAsync mqttClient;
    // MQTT connect options
    MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
    int rc;
    int ret;
    conn_opts.username = USERNAME;
    conn_opts.password = PASSWORD;
    
    printf("> user : %s.\n", USERNAME);
    printf("> pwd : %s.\n", PASSWORD);

    // Params : MqttHandle, ServerAddress, ClientID, IsPersistence, NULL
    ret = MQTTAsync_create(&mqttClient, SERVER_ADDRESS, CLIENT_ID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    if (ret != MQTTASYNC_SUCCESS) {
        printf("> x Cannot create mqtt client.\n");
        return -1;
    }
    printf("> O MQTTAsync created.\n");
    
    conn_opts.onSuccess = onConnect;
    conn_opts.keepAliveInterval = 20;

    rc = MQTTAsync_connect(mqttClient, &conn_opts);
    if (rc != MQTTASYNC_SUCCESS) {
        printf("> x Cannot connect server.\n");
        return -1;
    }
    printf("> O MQTTAsync connected.\n");

    while(1) {
        sleep(5);
    }
    
    return 0;
}
