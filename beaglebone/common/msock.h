#ifndef SOCKHEAD_H
#define SOCKHEAD_H 1

#include <stdio.h>   
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>	/* ULTRIX didn't like stat with types*/
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <ctype.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>  /* for inet_ntoa */
#include <time.h>  /* for ctime */
#include <signal.h>
#include <errno.h>
#include <sys/wait.h> 
#include <pwd.h>
#include <grp.h>   
#include <fcntl.h>
#include <limits.h>

/*
** socket functin prototypes
*/
int sockRead      (int,char *,size_t);
int sockWrite     (int,char *,size_t);

int ClientSocket  (char *netaddress,u_short port);
int ServerSocket  (u_short,int);
int getConnection (int, u_short,int *,int);
int makeConnection(char *,int,char *);

int sockGets      (int,char *,size_t);
int sockPuts      (int,char *);
int getHostByName (char *host_found,char *check_for);

int getPeerInfo   (int,char *,char *,u_short *);

int atoport       (char *,char *);
int iread         (int,char *,int);


struct in_addr *atoaddr(char *);

#endif /* SOCKHEAD_H*/
