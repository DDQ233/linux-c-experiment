# Compiler
CC = gcc

# Project objects
DEST = main
PROJECT_SRC = mysqlUtilTest.c
PROJECT_OBJ = mysqlUtilTest

# Util objects
## MySql
MYSQL_UTIL_SRC = mysqlutil.c
MYSQL_UTIL_OBJ = mysqlutil.o
MYSQL_UTIL_LIB = libmysqlutil.a
## Serial Port
SERIALPORT_SRC = serialportutil.c
SERIALPORT_OBJ = serialportutil.o
SERIALPORT_LIB = libserialportutil.a
## MQTT
MQTT_UTIL_SRC = mqttutil.c
MQTT_UTIL_OBJ = mqttutil.o
MQTT_UTIL_LIB = libmqttutil.a

# Main function Library
LIB += -L.
LIB += -lmysqlclient
LIB += -lmysqlutil
LIB += -lserialportutil
LIB += -lpaho-mqtt3a
# LIB += -lmqttutil


# Util Library
## MySql
MYSQL_LIB += -lmysqlutil
## MQTT
MQTT_LIB += -lpaho-mqtt3a

# Project head file path

# Util head file path
MYSQL_INCLUDE_PATH = -I/usr/include/mysql
MQTT_INCLUDE_PATH = -I/home/ddq/3rdlib/paho.mqtt/include/

# Main
$(DEST) :
	$(CC) -c -o $(MYSQL_UTIL_OBJ) $(MYSQL_UTIL_SRC) $(MYSQL_LIB)
	ar rcs $(MYSQL_UTIL_LIB) $(MYSQL_UTIL_OBJ)
	$(CC) -c -o $(SERIALPORT_OBJ) $(SERIALPORT_SRC)
	ar rcs $(SERIALPORT_LIB) $(SERIALPORT_OBJ)
	$(CC) $(PROJECT_SRC) -o $(PROJECT_OBJ) $(LIB)

clean:
	rm -f *.a *.o $(DEST) *~