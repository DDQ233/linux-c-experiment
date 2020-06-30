#ifndef __MQTTUTIL_H__
#define __MQTTUTIL_H__

// Bind connect options
MQTTAsync_connectOptions bindConnectOptions(
    char* username, 
    char* password, 
    int keepAliveInterval, 
    int isCleansession, 
    int isAutomaticReconnect, 
    MQTTAsync_onSuccess* onSuccess, 
    MQTTAsync_onFailure* onFailure);

// Bind resoponse options
MQTTAsync_responseOptions bindResponseOptions(
    MQTTAsync_onSuccess* onSuccess, 
    MQTTAsync_onFailure* onFailure
    );

// Bind disconnect options
MQTTAsync_disconnectOptions bindDisconnectOptions(
    MQTTAsync_onSuccess* onSuccess, 
    MQTTAsync_onFailure* onFailure);

// Create mqtt client handle.
MQTTAsync createClient(char* address, char* clientId);

// Set callback function.
MQTTAsync setCallbacks(
    MQTTAsync client, 
    MQTTAsync_connectionLost* connlost, 
    MQTTAsync_messageArrived* messageArrived, 
    MQTTAsync_deliveryComplete* deliveryComplete);

// Connect mqtt server
MQTTAsync connectMqttServer(MQTTAsync client, MQTTAsync_connectOptions conn_opts);

// Subscribe topic
MQTTAsync subscribeTopic(MQTTAsync client, char* topic, int qos);

#endif