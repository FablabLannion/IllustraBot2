/**
 * test client for arm_control server
 *********************************************************************
 * Fablab Lannion IllustraBot2 project
 * http://fablab-lannion.org/wiki/index.php?title=IllustraBot2
 *********************************************************************
 * Copyright: (c) 2013 Jérôme Labidurie
 * Licence:   GNU General Public Licence version 3
 * Email:     jerome.labidurie at gmail.com
 *********************************************************************
 * This file is part of IllustraBot2.
 * 
 * IllustraBot2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IllustraBot2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with IllustraBot2.  If not, see <http://www.gnu.org/licenses/>.
 *********************************************************************
 */

#include <msock.h>
#include <proto.h>
#include "joystick.h"

void make_message_data_wii (message_t* msg)
{
   msg->version = PROTO_VERSION;
   msg->size =  sizeof(message_t);
   msg->type = T_DATA_WII;
   
   msg->pl.nunchuk.b1 = 1;
   msg->pl.nunchuk.b2 = 1;
   msg->pl.nunchuk.x  = 1.1;
   msg->pl.nunchuk.y  = 2.1;
   msg->pl.nunchuk.z  = 3.1;
}

void make_message_data_joy (message_t* msg, joy_t* joy)
{
   msg->version = PROTO_VERSION;
   msg->size =  sizeof(message_t);
   msg->type = T_DATA_JOY;
   
   msg->pl.joystick.b1 = joy->button[4];
   msg->pl.joystick.b2 = joy->button[5];
   msg->pl.joystick.b3 = joy->button[6];
   msg->pl.joystick.b4 = joy->button[7];
   msg->pl.joystick.x1 = joy->axis[0];
   msg->pl.joystick.y1 = joy->axis[1];
   msg->pl.joystick.x2 = joy->axis[2];
   msg->pl.joystick.y2 = joy->axis[3];
}

void make_message_cmd (message_t* msg)
{
   msg->version = PROTO_VERSION;
   msg->size =  sizeof(message_t);
   msg->type = T_COMMAND;
   memset (msg->pl.txt, 0, sizeof(msg->pl.txt));
   strncpy (msg->pl.txt, "Hello world", 12);
   
}

int main (int argc,char **argv) 
{
   int
   rc,
   port,
   sock_fd;
   
   char
   szbuf[BUFSIZ],
   szhost[64];
   
   message_t msg;
   joy_t joy;
   
   if (argc < 2)
   {
      (void) fprintf(stderr,"usage: %s <host> <port>\n",argv[0]);
      (void) fprintf(stderr,"\nExample:\n");
      (void) fprintf(stderr," %s localhost 1337\n",argv[0]);
      return(1);
   }
   
   port = atoi(argv[2]);
   
   // init joystick
   if ( joy_init(&joy) != 0) {
      fprintf(stderr, "cannot find joystick\n");
      return 1;
   }
   joy_nonblocking(&joy);
   printf( "Joystick detected : %s\n\t%2d axis\n\t%2d buttons\n" ,
           joy.name, joy.num_of_axis , joy.num_of_buttons);
   
   
   /* open a connection to the server */
   sock_fd=ClientSocket(argv[1],(u_short) port);
   if (sock_fd == -1)
   {
      (void) fprintf(stderr,"Failed to connect to %s:%s\n", argv[1], argv[2]);
      return (1);
   }
   
   /* we connected successfully, send command*/
   make_message_cmd(&msg);
   rc = sockWrite (sock_fd, (char*) &msg, msg.size);
   printf ("%d bytes written\n",rc);
   
   /* print answer came from server */
   memset (szbuf, 0, BUFSIZ);
   if (sockGets(sock_fd,szbuf,sizeof(szbuf)-1)) {
      (void) fprintf(stdout,"%s\n",szbuf);
      (void) fflush(stdout);
   }
   
   while (1) {
      // read the joystick 
      joy_read(&joy);
      // send message to the server
      make_message_data_joy (&msg, &joy);
      rc = sockWrite (sock_fd, (char*) &msg, msg.size);
      printf ("%d bytes written\n",rc);
      
      /* print answer came from server */
      memset (szbuf, 0, BUFSIZ);
      if (sockGets(sock_fd,szbuf,sizeof(szbuf)-1)) {
         (void) fprintf(stdout,"%s\n",szbuf);
         (void) fflush(stdout);
      }
      // one command every 10ms
      usleep(10000);
   }
   
   close(sock_fd);
   return(0);
}


