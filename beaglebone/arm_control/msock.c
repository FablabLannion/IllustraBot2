/*
 *  socket utility routines
 *
 *  RCS:
 *      $Revision: 1.1.1.1 $
 *      $Date: 1997/05/26 22:43:03 $
 *
 *  Description:
 *      most of the functions are taken (in some cases modified) from the
 *      Internet Socket Programming FAQ example code.
 *
 *  Development History:
 *      who                 when        why
 *      muquit@semcor.com   21-Mar-96   first cut
 *      jerome@labidurie.fr 20/01/2014  remove fork
 */

#include "msock.h"

#define LINE_FEED       0x0a
#define CARRIAGE_RETURN 0x0d

/*
** sockRead()
** like read() system call, except that it will make sure that all
** the data foes through the socket
** taken from steven's network programming (page 279)
*/

int XXsockRead(int sockfd,char *buf,size_t nbytes)
{
    int
        nleft,
        nread;

    nleft=nbytes;

    while (nleft > 0)
    {
        nread=read(sockfd,buf,nleft);
        if (nread < 0)                      /* error */
            return (-1);
        else if (nread == 0)                /* EOF */
            break;

        nleft -= nread;
        buf += nread;
    }
    return (nbytes-nleft);                  /* return >= 0 */
}

/*
** this function writes a character string out to a socket.
** returns
**      number of bytes written to the socket
**      -1 if the connection is closed while it is trying to write
*/
int sockPuts(int sockfd,char *str)
{
    int
        rc;

    rc=sockWrite(sockfd,str,strlen(str));
    return(rc);
}

int sockWrite(int sockfd,char *str,size_t count)
{
    size_t
        bytesSent=0;

    int
        thisWrite;

    while (bytesSent < count)
    {
       do
          thisWrite=write(sockfd,str,count-bytesSent);
       while ((thisWrite < 0) && (errno == EINTR));

       if (thisWrite <= 0)
          return (thisWrite);

        bytesSent += thisWrite;
        str += thisWrite;
    }
    return (count);
}

/*
** getServerSocket()
** a wrapper around getConnection() for simplicity
** it's only for stream socket
** port is in host byte order
*/
int ServerSocket(u_short port,int max_server)
{
    int
        dummy=(-1);

    int
        sock_fd;

    u_short
        nport;

    /*
    ** convert port to netword byte word
    */
    nport=htons(port);
    sock_fd=getConnection(SOCK_STREAM,nport,&dummy,max_server);

    return (sock_fd);
}
/*
** this function listens on a port and returns connecions.
** it forks returns off internally, so the calling funcion does not
** have to worry about that.
** the function will create a new process for every incomming connection,
** so in the listening process, it will never return. Only when a connection
** comes in, and we create a process for it, will the function return.
** THE CALLING FUNCION SHOULD NOT LOOP
*/
int getConnection(int socket_type,u_short port,int *listener,int max_serv)
{
    struct sockaddr_in
        address;

    int
        listeningSocket;

    int
        connectedSocket = -1;

    int
        newProcess;

    int
        one = 1;

    /*
    ** setup internet address information
    ** this is used with the bind () call
    */
    memset((char *) &address,0,sizeof(address));
    address.sin_family=AF_INET;
    address.sin_port=port;
    address.sin_addr.s_addr=htonl(INADDR_ANY);

    listeningSocket=socket(AF_INET,socket_type,0);
    if (listeningSocket < 0)
    {
        (void) fprintf (stderr,"Unable to open socket!\n");
        perror("socket");
        exit (1);
    }

    if (listener != (int *) NULL)
       *listener=listeningSocket;

    /*
    ** we will resuse port
    */
    setsockopt(listeningSocket,SOL_SOCKET,SO_REUSEADDR,(char *) &one,
        sizeof(int));

    /*
    ** we will set keep alive.  read Steven, Network Prog. 2nd edition, p186
    */
    /*
    one=1;
    setsockopt(listeningSocket,SOL_SOCKET,SO_KEEPALIVE,(char *) &one,
            sizeof(int));
    */

    if (bind(listeningSocket,(struct sockaddr *) &address,
        sizeof(address)) < 0)
    {
        (void) fprintf (stderr,"Unable to bind to socket\n");
        (void) fprintf (stderr,"Probably the port is already in use!\n");
        (void) fprintf(stderr,"Or you do not have permission to bind!\n");
        close(listeningSocket);
        exit (1);
    }

    if (socket_type == SOCK_STREAM)
    {
        /*
        ** queue up max_serv connections before having them
        ** automatically rejected
        */
        listen(listeningSocket,max_serv);

        while (connectedSocket < 0)
        {
            connectedSocket=accept(listeningSocket,NULL,NULL);
            if (connectedSocket < 0)
            {
                /*
                ** either a real error occured or blocking was
                ** interrupted for some reason. only abort execution
                ** if a real error occured
                */
                if (errno != EINTR)
                {
                    (void) fprintf (stderr,"unable to accept!\n");
                    perror("accept");
                    close (listeningSocket);
                    exit (1);
                }
                else
                    continue; /* don't fork, do the accept again*/
            }
//             newProcess=fork();
newProcess = 0;
            if (newProcess < 0)
            {
                (void) fprintf (stderr,"failed to fork!\n");
                perror("fork");
                close(connectedSocket);
                connectedSocket=(-1);
            }
            else
            {
                /*
                ** we have a new process (child)
                */
                if (newProcess == 0)
                {
                    /*
                    ** this is the new process
                    ** close our copy of the socket
                    */
                    close(listeningSocket);

                    *listener=(-1); /* closed in this process, we are not
                                    ** responsible for it
                                    */
                }
                else
                {
                    /*
                    ** this is the main loop. close copy of connected
                    ** socket, and continue loop
                    */
                    close (connectedSocket);
                    connectedSocket=(-1);
                }
            }
        }
        return (connectedSocket);
     }
     else
        return (listeningSocket);
}

int sockGets(int sockfd,char *str,size_t count)
{
    int
        bytesRead;

    int
        totalCount=0;

    char
        *currentPosition;

    char
        lastRead=0;

    currentPosition=str;

    while (lastRead != LINE_FEED)
    {
        bytesRead=read(sockfd,&lastRead,1);
        if (bytesRead <= 0)
        {
            /*
            ** the other side may have closed unexpectedly
            */
            return (-1);
        }
        if ((totalCount < count) && (lastRead != LINE_FEED)
                                 && (lastRead != CARRIAGE_RETURN))
        {
            *currentPosition=lastRead;
            currentPosition++;
            totalCount++;
        }
    }
    if (count > 0)
        *currentPosition=0;

    return (totalCount);
}

/*
** return params: clie_host,clie_ip,cli_port
** function returns 0 on success and -1 on failure
*/
/*
int getPeerInfo(int sockfd,char cli_host[],char cli_ip[],u_short *cli_port)
*/
int getPeerInfo(int sockfd,char *cli_host,char *cli_ip,u_short *cli_port)
{
    struct sockaddr_in
        cs;
    int
        len;
    struct hostent
        *ch;

    len=sizeof(cs);

    if (getpeername(sockfd,(struct sockaddr *) &cs,&len) < 0)
    {
        /*
        logError("Unable to obtain address of client",(char *) NULL);
        */
        return (-1);
    }
    /*
    ** extract client IP
    */
    (void) strcpy(cli_ip,inet_ntoa(cs.sin_addr));

    /*
    ** extract clinet_port
    */
    *cli_port=ntohs(cs.sin_port);

    /*
    ** extract cononical name of the client
    */
    ch=gethostbyaddr((char *) &cs.sin_addr,sizeof(cs.sin_addr),AF_INET);
    if (ch == NULL)
    {
        /*
        logError("Unable to get cononical name of the client",(char *)NULL);
        */
        return (-1);
    }
    else
    {
        (void) strcpy(cli_host,ch->h_name);
    }

return (0);
}

/*
** ClientSocket()
** a wrapper around makeConnection() for Stream socket
** returns the socket on success (a positive number), -1 on failure
** NOTE: port is in host byte order
*/
int ClientSocket(char *netaddress,u_short port)
{
    struct in_addr
        *addr;

    struct sockaddr_in
        address;

    int
        connected,
        sockfd;

    int
        nport;

    addr=atoaddr(netaddress);
    if (addr == (struct in_addr *) NULL)
    {
        (void) fprintf (stderr,"ClientSocket(): Invalid network address\n");
        return (-1);
    }

    /*
    ** convert port to network byte order first
    */
    nport=htons(port);
    if (nport == -1)
    {
        (void) fprintf (stderr,"Failed to convert port %d to netword byte order!\n",port);
        return (-1);
    }

    memset((char *) &address,0,sizeof(address));
    address.sin_family=AF_INET;
    address.sin_port=nport;
    address.sin_addr.s_addr=addr->s_addr;

    sockfd=socket(AF_INET,SOCK_STREAM,0);

    /*
    ** connect
    */
    connected=connect(sockfd,(struct sockaddr *) &address,
        sizeof(address));

    if (connected < 0)
    {
        perror("connect");
        (void) fprintf (stderr,"connect() failed!\n");
        return (-1);
    }

    return (sockfd);
}

/*
** function to make a connection to a given server/port
** type is SOCK_STREAM or SOCK_DGRAM
** netaddress is host to connect to
** the functin returns the socket ready to use.
*/

int makeConnection(char *service,int type,char *netaddress)
{
    int
        port=(-1);

    struct in_addr
        *addr;

    int
        sockfd,
        connected;

    struct sockaddr_in
        address;

    if (type == SOCK_STREAM)
        port=atoport(service,"tcp");

    if (type == SOCK_DGRAM)
        port=atoport(service,"udp");

    if (port == -1)
    {
        (void) fprintf (stderr,"makeConnection: Invalid socket type\n");
        return (-1);
    }

    addr=atoaddr(netaddress);
    if (addr == (struct in_addr *) NULL)
    {
        (void) fprintf (stderr,"makeConnection: Invalid network address\n");
        return (-1);
    }

    memset((char *) &address,0,sizeof(address));
    address.sin_family=AF_INET;
    address.sin_port=port;
    address.sin_addr.s_addr=addr->s_addr;

    sockfd=socket(AF_INET,type,0);

    if (type == SOCK_STREAM)
    {
        connected=connect(sockfd,(struct sockaddr *) &address,
            sizeof(address));

        if (connected < 0)
        {
            perror("connect");
            return (-1);
        }

        return (sockfd);
    }

    /*
    ** must be udp
    */

    if (bind(sockfd,(struct sockaddr *) &address,sizeof(address)) < 0)
    {
        perror ("bind");
        return (-1);
    }
    return (sockfd);
}

/*
** function returns  a portnumber. it takes service name and service
** type as input arguments. if the service is not found, it tries it
** as a decimal number. The number returned is byte ordered for the
** network
*/

int atoport(char *service,char *proto)
{
    int
        port;

    long int
        lport;

    struct servent
        *serv;

    char
        *errpos;

    /*
    ** first try to read it from /etc/services
    */

    serv=getservbyname(service,proto);

    if (serv != (struct servent *) NULL)
        port=serv->s_port;
    else
    {
        /*
        ** it's a number
        */
        lport=strtol(service,&errpos,0);
        if ((errpos[0] != 0) || (lport < 1) || (lport > 65535))
            return (-1);
        port=htons(lport);
    }
    return (port);
}

struct in_addr *atoaddr(char *address)
{
    struct hostent
        *host;

    static struct in_addr
        saddr;

    saddr.s_addr=inet_addr(address);
    if (saddr.s_addr != -1)
        return (&saddr);

    host=gethostbyname(address);
    if (host != (struct hostent *) NULL)
        return ((struct in_addr *) *host->h_addr_list);

    return ((struct in_addr *) NULL);
}


/*
** iread()
** same as read() system call except for automatic
** recovery for EINTR errors
** from llnl xdir
*/

int iread(int fd,char *buf,int nbyte)
{
    int
        ret=0;

    while (1)
    {
        ret=read(fd,buf,nbyte);
        if ((ret < 0) && (errno == EINTR))
            continue;
        else
            return (ret);
    }

    return (ret);   /* won't be here */
}

int sockRead(int sockfd,char *buf,size_t n)
{
    int
        total_nbytes=0;

    int
        nbytes;

    int
        saved_errno;


    while (total_nbytes < n)
    {
        nbytes=iread(sockfd,&buf[total_nbytes],
            n-total_nbytes);
        if (nbytes < 0)
        {
            saved_errno=errno;
            if (saved_errno == ECONNRESET)
            {
                return (-3);
            }
            else
                return (-1);
        }
        else if (nbytes == 0)
            return (total_nbytes);
        else
           total_nbytes += nbytes;
    }
    return (total_nbytes);
}


/*
 *  getHostByName ()
 *      checks and returns the real hostname+domain if a hostname or
 *      hostname+domain is specified.
 *  RCS:
 *      $Revision$
 *      $Date$
 *
 *  Description:
 *      This function real hostname + domain if a hostname or hostname +
 *      domain name is specified. For example, if a hostname in fccc
 *      domain called "aquila" is passed, the function will resolve it to
 *      the real hostname "aquila.rm.fccc.edu". If "aquila.rm.fccc.edu" is
 *      passwd, it will also be resolved to ""aquila.rm.fccc.edu". If
 *      "sfdmaster" is passed, it will also resolve to the real hostname
 *      designated as the sfd master host. Note: the asked host does not
 *      have to be a real host, it can be a CNAME. But no host will be
 *      found if the asked host is an MX record.
 *
 *  Parameters:
 *      host_found      if host is resolved successfully, the string will
 *                      hold the real host+domain name. (returns).
 *      check_for       try to resolve this name
 *
 *  Return Values:
 *      0       if asked host was resolved successfully
 *      -1      if the asked host does not exist.
 *
 *
 *  Side Effects:
 *      host_found is changed.
 *
 *
 *  Limitations and Comments:
 *      DNS server must be working properly in order this routine to work
 *      successfully. host_found must have enough space to hold the found
 *      host+domain name.
 *
 *  Development History:
 *      who                  when       why
 *      ma_muquit@fccc.edu   Sep-22-97  first cut
 */

int getHostByName (char *host_found,char *check_for)
{
    struct hostent
        *h;

    int
        rc=(-1);

    *host_found='\0';

    if (check_for == (char *) NULL)
        rc=(-1);

    h=gethostbyname(check_for);
    if (h != (struct hostent *) NULL)
    {
        rc=0;
        (void) strcpy(host_found,h->h_name);
    }
    else
        rc=(-1);

    return(rc);
}


/*
** read/write routines for SSL
*/

#ifdef USE_SSL

int ireadSSL(SSL *ssl,char *buf,int nbyte)
{
    int
        ret=0;

    while (1)
    {
        ret=SSL_read(ssl,buf,nbyte);
        if ((ret < 0) && (errno == EINTR))
            continue;
        else
            return (ret);
    }

    return (ret);   /* won't be here */
}

int sockReadSSL(SSL *ssl,char *buf,size_t n)
{
    int
        total_nbytes=0;

    int
        nbytes;

    int
        saved_errno;


    while (total_nbytes < n)
    {
        nbytes=ireadSSL(ssl,&buf[total_nbytes],
            n-total_nbytes);
        if (nbytes < 0)
        {
            saved_errno=errno;
            if (saved_errno == ECONNRESET)
            {
                return (-3);
            }
            else
                return (-1);
        }
        else if (nbytes == 0)
            return (total_nbytes);
        else
           total_nbytes += nbytes;
    }
    return (total_nbytes);
}

/*
** this function writes a character string out to a socket.
** it returns -1 if the connection is closed while it is trying to
** write
*/
int sockPutsSSL(SSL *ssl,char *str)
{
    return (sockWriteSSL(ssl,str,strlen(str)));
}

int sockWriteSSL(SSL *ssl,char *str,size_t count)
{
    size_t
        bytesSent=0;

    int
        thisWrite;

    while (bytesSent < count)
    {
       do
          thisWrite=SSL_write(ssl,str,count-bytesSent);
       while ((thisWrite < 0) && (errno == EINTR));

       if (thisWrite <= 0)
          return (thisWrite);

        bytesSent += thisWrite;
        str += thisWrite;
    }
    return (count);
}

int sockGetsSSL(SSL *ssl,char *str,size_t count)
{
    int
        bytesRead;

    int
        totalCount=0;

    char
        *currentPosition;

    char
        lastRead=0;

    currentPosition=str;

    while (lastRead != 10)
    {
        bytesRead=SSL_read(ssl,&lastRead,1);
        if (bytesRead <= 0)
        {
            /*
            ** the other side may have closed unexpectedly
            */
            return (-1);
        }
        if ((totalCount < count) && (lastRead != 10)
            && (lastRead != 13))
        {
            *currentPosition=lastRead;
            currentPosition++;
            totalCount++;
        }
    }
    if (count > 0)
        *currentPosition=0;

    return (totalCount);
}


#endif  /* USE_SSL */
