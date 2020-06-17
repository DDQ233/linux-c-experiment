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
    MQTTAsync_onFailure* onFailure);
{
    MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
    
    conn_opts.username = username;
    conn_opts.password = password;
    conn_opts.keepAliveInterval = keepAliveInterval;
    conn_opts.cleansession = isCleansession;
    conn_opts.automaticReconnect = isAutomaticReconnect;
    conn_opts.onSuccess = onSuccess;
    conn_opts.onFailure = onFailure;

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

    return resp_opts;
}

// Bind disconnect options
MQTTAsync_disconnectOptions bindResponseOptions(
    MQTTAsync_onSuccess* onSuccess, 
    MQTTAsync_onFailure* onFailure)
{
    MQTTAsync_disconnectOptions disc_opts = MQTTAsync_disconnectOptions_initializer;

    disc_opts.onSuccess = onSuccess;
    disc_opts.onFailure = onFailure;

    return disc_opts;
}

MQTTAsync createClient(char* address, char* clientId)
{
    MQTTAsync mqttClient;
    int ret;

    if ((ret = MQTTAsync_create(&client, address, clientId, MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTASYNC_SUCCESS) {
        printf("> x Cannot create mqtt-async client.\n");
        printf("> x Error : %d.\n", ret);
    } else {
        printf("> O Create mqtt-async client handle successfully.\n");
    }
    
    return mqttClient;
}

MQTTAsync setCallbacks(MQTTAsync client, )
