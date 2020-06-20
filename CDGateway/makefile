CC = gcc

# Project
DESC = main
PROJECT_SRC = main.c
PROJECT_OBJ = main

# Serial Port Util
SERIALPORT_UTIL_SRC = serialportUtil.c
SERIALPORT_UTIL_OBJ = serialportUtil.o
SERIALPORT_UTIL_LIB = libserialportutil.a

# Serial port lib
SERIALPORT_LIB += -lpthread

# Project lib
LIB += -L.
LIB += -lserialportutil



$(DESC) : 
	$(CC) -c -o $(SERIALPORT_UTIL_OBJ) $(SERIALPORT_UTIL_SRC) $(SERIALPORT_LIB)
	ar rsc $(SERIALPORT_UTIL_LIB) $(SERIALPORT_UTIL_OBJ)
	$(CC) $(PROJECT_SRC) -o $(PROJECT_OBJ) $(LIB)

clean:
	rm -f *.a *.o $(DEST) *~