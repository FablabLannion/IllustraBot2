
INCLUDES= -I. -I../common
CC= gcc
MSOCKDEFS= -DSTDC_HEADERS=1 -DHAVE_SYS_WAIT_H=1 -DTIME_WITH_SYS_TIME=1 -DHAVE_STRING_H=1 -DHAVE_FCNTL_H=1 -DHAVE_MEMORY_H=1 -DHAVE_MALLOC_H=1 -DHAVE_UNISTD_H=1 -DHAVE_CTYPE_H=1 -DHAVE_STRINGS_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_SYS_PARAM_H=1 -DHAVE_SYS_FILE_H=1 -DHAVE_STDLIB_H=1 -DHAVE_FLOCK=1 -DHAVE_STRCASECMP=1 -DHAVE_MKTIME=1 -DHAVE_STRFTIME=1 -DHAVE_GMTIME=1
DEFS=${MSOCKDEFS} -D_REENTRANT
PROGNAME=server

LIBS=-lpthread

DEFINES= $(INCLUDES) $(DEFS)
CFLAGS= -g -O -Wall $(DEFINES)


OBJS= main.o ../common/msock.o SimpleGPIO.o easydriver.o

.c.o:
	rm -f $@
	$(CC) $(CFLAGS) -c $*.c -o $*.o

all: $(PROGNAME)

$(PROGNAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROGNAME) $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS) $(PROGNAME)

# compile device tree overlay
dtbo: arm_control-1.dts
	dtc -O dtb -o arm_control-1-00A0.dtbo -b 0 -@ arm_control-1.dts
