#ifndef __MQTTUTIL_H__
#define __MQTTUTIL_H__

MQTTAsync createClient(char* address, char* clientId);
MQTTAsync setCallbacks(MQTTAsync mqttClient, MQTTAsync_connectionLost* connlost, MQTTAsync_messageArrived* msgarrvd);
void bindConnOptions(MQTTAsync mqttClient, int keepAliveInterval, int cleansession, MQTTAsync_onSuccess* onSuccess, MQTTAsync_onFailure* onFailure, void* context);
void bindDisconnOptions()
MQTTAsync connectMqttServer(MQTTAsync mqttClient, MQTTAsync_connectOptions options);
disconnectMqttServer(MQTTAsync mqttClient, );


#endif