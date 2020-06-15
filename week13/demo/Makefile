#SRCS=$(wildcard *.c)
#OBJECTS=$(patsubst %.c,%.o,$(SRCS))
#.PHONY:all
#all:
#	@echo $(SRCS)
#	@echo $(OBJECTS)
#getmax.exe:main.o max.o
#	gcc -o getmax.exe main.o max.o 
#main.o:main.c
#	gcc -c main.c  #<=>gcc -o main.o main.c
#max.o:max.c
#	gcc -c max.c
#.PHONY:clean
#clean:
#	rm -f   *.o   *.exe    *~
# "*"是表示文件系统中的通配符，“%”是表示Makefile变量中的通配符
SRCS=$(wildcard *.c)#将当前目录下的c源文件全部获取，SRCS:  max.c  main.c
OBJECTS=$(patsubst %.c,%.o,$(SRCS))#获取了c源文件形成的.o文件，OBJECTS: max.o  main.o
DEST=test#对于具体的工程只需修改为要生成最终可执行文件名即可
CC=xxx-gcc#在不同的cpu+linux，只需更换为对应gcc即可

MYLIBS+=-L xxx -lxx

MYINCLUDEPATH= -I xxxx#-I:添加新的头文件搜索路径

$(DEST):$(OBJECTS)
	$(CC) -o   $@   $^ $(MYLIBS)
%.o:%.c# “%”是一个Makefile的通配符，它代表任意的字符串
	$(CC) -c  -o $@  $^ $(MYINCLUDEPATH)
.PHONY:clean
clean:
	rm -f   *.o   $(DEST)    *~
