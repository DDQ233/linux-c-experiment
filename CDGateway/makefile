CC = gcc

# Project
DESC = main
PROJECT_SRC = main.c
PROJECT_OBJ = main

# Serial Port Util
SERIALPORT_UTIL_SRC = serialportutil.c
SERIALPORT_UTIL_OBJ = serialportutil.o
SERIALPORT_UTIL_LIB = libserialportutil.a

# Mysql Util
MYSQL_UTIL_SRC = mysqlutil.c
MYSQL_UTIL_OBJ = mysqlutil.o
MYSQL_UTIL_LIB = libmysqlutil.a

# Mqtt Util
MQTT_UTIL_SRC = mqttutil.c
MQTT_UTIL_OBJ = mqttutil.o
MQTT_UTIL_LIB = libmqttutil.a

# Util lib
SERIALPORT_LIB += -lpthread
MYSQL_LIB += -lmysqlclient
MQTT_LIB += -lpaho-mqtt3a

# Project lib
LIB += -L.
LIB += -lserialportutil
LIB += -lpthread
LIB += -lmysqlutil
LIB += -lmysqlclient
LIB += -lmqttutil
LIB += -lpaho-mqtt3a
LIB += -lcjson


$(DESC) : 
	$(CC) -c -o $(SERIALPORT_UTIL_OBJ) $(SERIALPORT_UTIL_SRC) $(SERIALPORT_LIB)
	ar rsc $(SERIALPORT_UTIL_LIB) $(SERIALPORT_UTIL_OBJ)
	$(CC) -c -o $(MYSQL_UTIL_OBJ) $(MYSQL_UTIL_SRC) $(MYSQL_LIB)
	ar rsc $(MYSQL_UTIL_LIB) $(MYSQL_UTIL_OBJ)
	$(CC) -c -o $(MQTT_UTIL_OBJ) $(MQTT_UTIL_SRC) $(MQTTLIB)
	ar rsc $(MQTT_UTIL_LIB) $(MQTT_UTIL_OBJ)
	$(CC) $(PROJECT_SRC) -o $(PROJECT_OBJ) $(LIB)

clean:
	rm -f *.a *.o $(DESC) *~