# Generated automatically from Makefile.in by configure.
###############################################################
# Makefile for hget
# muquit, Jul-10-1999
#####

INCLUDES= -I. -I../common -I../common/libmsock
CC= gcc
# DEFS=  -DSTDC_HEADERS=1 -DHAVE_SYS_WAIT_H=1 -DTIME_WITH_SYS_TIME=1 -DHAVE_STRING_H=1 -DHAVE_FCNTL_H=1 -DHAVE_MEMORY_H=1 -DHAVE_MALLOC_H=1 -DHAVE_UNISTD_H=1 -DHAVE_CTYPE_H=1 -DHAVE_STRINGS_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_SYS_PARAM_H=1 -DHAVE_SYS_FILE_H=1 -DHAVE_STDLIB_H=1 -DHAVE_FLOCK=1 -DHAVE_STRCASECMP=1 -DHAVE_MKTIME=1 -DHAVE_STRFTIME=1 -DHAVE_GMTIME=1  -DSYS_UNIX=1 -DDEBUG=1
PROGNAME=client

LIBS=  -L../common/libmsock -lmsock

DEFINES= $(INCLUDES) $(DEFS)
CFLAGS= -g -O2 -Wall $(DEFINES) 


OBJS= main.o joystick.o

.c.o:
	rm -f $@
	$(CC) $(CFLAGS) -c $*.c

all: $(PROGNAME)

$(PROGNAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROGNAME) $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS) $(PROGNAME)
