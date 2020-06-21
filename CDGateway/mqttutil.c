#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <MQTTAsync.h>
#include "mqttutil.h"

// Bind connect options
MQTTAsync_connectOptions bindConnectOptions(
    char* username, 
    char* password, 
    int keepAliveInterval, 
    int isCleansession, 
    int isAutomaticReconnect, 
    MQTTAsync_onSuccess* onSuccess, 
    MQTTAsync_onFailure* onFailure)
{
    MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
    
    conn_opts.username = username;
    conn_opts.password = password;
    conn_opts.keepAliveInterval = keepAliveInterval;
    conn_opts.cleansession = isCleansession;
    conn_opts.automaticReconnect = isAutomaticReconnect;
    conn_opts.onSuccess = onSuccess;
    conn_opts.onFailure = onFailure;

    printf("> O Bind connect options successfully.\n");

    return conn_opts;
}

// Bind resoponse options
MQTTAsync_responseOptions bindResponseOptions(
    MQTTAsync_onSuccess* onSuccess, 
    MQTTAsync_onFailure* onFailure)
{
    MQTTAsync_responseOptions resp_opts = MQTTAsync_responseOptions_initializer;

    resp_opts.onSuccess = onSuccess;
    resp_opts.onFailure = onFailure;

    printf("> O Bind response options successfully.\n");

    return resp_opts;
}

// Bind disconnect options
MQTTAsync_disconnectOptions bindDisconnectOptions(
    MQTTAsync_onSuccess* onSuccess, 
    MQTTAsync_onFailure* onFailure)
{
    MQTTAsync_disconnectOptions disc_opts = MQTTAsync_disconnectOptions_initializer;

    disc_opts.onSuccess = onSuccess;
    disc_opts.onFailure = onFailure;

    printf("> O Bind disconnect options successfully.\n");

    return disc_opts;
}

// Create mqtt client handle.
MQTTAsync createClient(char* address, char* clientId)
{
    MQTTAsync mqttClient;
    int ret;

    if ((ret = MQTTAsync_create(&mqttClient, address, clientId, MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTASYNC_SUCCESS) {
        printf("> x Cannot create mqtt-async client.\n");
        printf("> x Error code : %d.\n", ret);
    } else {
        printf("> O Create mqtt-async client handle successfully.\n");
    }
    
    return mqttClient;
}

// Set callback function.
MQTTAsync setCallbacks(
    MQTTAsync client, 
    MQTTAsync_connectionLost* connlost, 
    MQTTAsync_messageArrived* messageArrived, 
    MQTTAsync_deliveryComplete* deliveryComplete)
{
    MQTTAsync mqttClient = client;
    int ret;
    if ((ret = MQTTAsync_setCallbacks(client, client, connlost, messageArrived, NULL)) != MQTTASYNC_SUCCESS)
	{
        printf("> x Cannot set callbacks function.\n");
        printf("> x Error code : %d.\n", ret);
	} else {
        printf("> O Set callbacks function successfully.\n");
    }
    return client;
}

// Connect mqtt server
MQTTAsync connectMqttServer(MQTTAsync client, MQTTAsync_connectOptions conn_opts)
{
    MQTTAsync mqttClient = client;
    int ret;
    if ((ret = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS) {
        printf("> x Cannot connect mqtt server.\n");
        printf("> x Error code : %d.\n", ret);
    } else {
        printf("> O Connect mqtt server successfully.\n");
    }
    return mqttClient;
}


