# Obviously Makefile
CC = g++
CFLAGS = -Wall
EXEC_NAME = motor_test
INCLUDES =
LIBS = -lncurses
OBJ_FILES = main.o
INSTALL_DIR = /usr/bin

all : $(EXEC_NAME)

clean :
	rm $(EXEC_NAME) $(OBJ_FILES)

$(EXEC_NAME) : $(OBJ_FILES)
	$(CC) -o $(EXEC_NAME) $(OBJ_FILES) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

%.o: %.cc
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

%.o: %.c
	gcc $(CFLAGS) $(INCLUDES) -o $@ -c $<

dtbo: motor_test-1.dts
	dtc -O dtb -o motor_test-1-00A0.dtbo -b 0 -@ motor_test-1.dts

install :
	cp $(EXEC_NAME) $(INSTALL_DIR)
