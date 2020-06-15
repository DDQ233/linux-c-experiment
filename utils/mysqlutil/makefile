SRCS=$(wildcard *.c)#将当前目录下的c源文件全部获取，SRCS:  max.c  main.c
OBJECTS=$(patsubst %.c,%.o,$(SRCS))#获取了c源文件形成的.o文件，OBJECTS: max.o  main.o
DEST=mysqlUtilTest#对于具体的工程只需修改为要生成最终可执行文件名即可
CC=gcc#在不同的cpu+linux，只需更换为对应gcc即可

# MYLIBS+=-L/usr/lib64/mysql
MYLIBS+=-L.

MYLIBS+=-lmysqlclient
MYLIBS+=-lmysqlutil

#-I:添加新的头文件搜索路径
# MYINCLUDEPATH= -I/usr/include/mysql

$(DEST):$(OBJECTS)
	$(CC) -o $@ $^ $(MYLIBS)
%.o:%.c# “%”是一个Makefile的通配符，它代表任意的字符串
	$(CC) -c -o $@ $^
.PHONY:clean

clean:
	rm -f *.o $(DEST) *~