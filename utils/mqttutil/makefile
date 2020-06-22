# SRCS=$(wildcard *.c)#将当前目录下的c源文件全部获取，SRCS:  max.c  main.c
# OBJECTS=$(patsubst %.c,%.o,$(SRCS))#获取了c源文件形成的.o文件，OBJECTS: max.o  main.o
# DEST=mqttUtilTest#对于具体的工程只需修改为要生成最终可执行文件名即可
CC=gcc#在不同的cpu+linux，只需更换为对应gcc即可

# Project
DESC = mqttUtilTest
PROJECT_SRC = mqttUtilTest.c
PROJECT_OBJ = mqttUtilTest

# MQTT Util
MQTT_UTIL_SRC = mqttutil.c
MQTT_UTIL_OBJ = mqttutil.o
MQTT_UTIL_LIB = libmqttutil.a

LIB+=-L.
LIB+=-lmqttutil
LIB+=-lpaho-mqtt3a

MQTTLIB+=-lpaho-mqtt3a

$(DESC) : 
	$(CC) -c -o $(MQTT_UTIL_OBJ) $(MQTT_UTIL_SRC) $(MQTTLIB)
	ar rsc $(MQTT_UTIL_LIB) $(MQTT_UTIL_OBJ)
	$(CC) $(PROJECT_SRC) -o $(PROJECT_OBJ) $(LIB)

clean:
	rm -f *.a *.o $(DESC) *~
