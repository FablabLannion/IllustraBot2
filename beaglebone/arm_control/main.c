/**
 * arm_control server
 *********************************************************************
 * Fablab Lannion IllustraBot2 project
 * http://fablab-lannion.org/wiki/index.php?title=IllustraBot2
 *********************************************************************
 * Copyright: (c) 2014 Jérôme Labidurie
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
 * TODO: handler for SIGINT
 */

#include "msock.h"
#include <proto.h>
#include "easydriver.h"
#include "main.h"


#include <pthread.h>
#include <inttypes.h>

motor_t motors[NB_MOTORS];      /**< array with all controled motors */

/** Display a received message from the client
 *
 * "Pretty" print of the message. display differs followinf message type.
 *
 * @param msg the message to dump
 */
void dump_message (message_t* msg)
{
   printf ("received message:\n");
   printf (" ver:%d, size:%d, type:%d\n", msg->version, msg->size, msg->type);
   switch (msg->type) {
      case T_DATA_WII:
         printf (" b1:%d, b2:%d\n", msg->pl.nunchuk.b1, msg->pl.nunchuk.b2);
         printf (" x:%3.3f, y:%3.3f, z:%3.3f\n", msg->pl.nunchuk.x, msg->pl.nunchuk.y, msg->pl.nunchuk.z);
         break;
      case T_DATA_JOY:
         printf (" b1:%d, b2:%d, b3:%d, b4:%d\n",
                 msg->pl.joystick.b1, msg->pl.joystick.b2, msg->pl.joystick.b3, msg->pl.joystick.b4);
         printf (" x1:%6d, y1:%6d, x2:%6d, y2:%6d,\n",
                 msg->pl.joystick.x1, msg->pl.joystick.y1, msg->pl.joystick.x2, msg->pl.joystick.y2);
         break;
      case T_COMMAND:
         printf (" command: %s\n",msg->pl.txt);
         break;
      default:
         printf ("unknown message type:%d\n",msg->type);
         break;
   }
} // dump_message

/** Display a received message from the client as hex bytes
 * @param msg the message to dump
 */
void hex_dump_message (message_t* msg)
{
   char* ptr = (char*)msg;
   int i;

   for (i=0;ptr < (char*)msg+sizeof(message_t); ptr++,i++) {
      printf ("%X ",*ptr);
   }
}// hex_dump_message

/** Convert a value from one range to another
 *
 * @param value the value to be remapped
 * @param in_min lower boundary of value range
 * @param in_max higher boundary of value range
 * @param out_min lower boundary of returned value range
 * @param out_max higher boundary of returned value range
 * @return the value mapped from [in-min-in_max] to [out_min-out_max]
 */
int map (int value, int in_min, int in_max, int out_min, int out_max)
{
   return (abs(value) - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}//map

/** Motor control function
 *
 * Must be executed in a separate thread
 *
 * @param pdata the motor number as an int
 */
void* run_motor (void* pdata)
{
   intptr_t nMotor = (intptr_t)pdata; // motor number
   int steps = 0;

   printf ("Thread %d, motor %" PRIdPTR ", starting ...\n", (int)pthread_self(), nMotor);
   while (1) {
      pthread_mutex_lock (&motors[nMotor].mutex);
      pthread_cond_wait (&motors[nMotor].cond, &motors[nMotor].mutex);

      steps = motors[nMotor].steps;
      motors[nMotor].steps = 0;
      pthread_mutex_unlock(&motors[nMotor].mutex);

      if (steps != 0) {
         // WARNING: here we will read speed outside of mutex !!
         ed_step (&motors[nMotor].ed, steps);
      }
   }
} // run_motor

/** Initialisation of motors
 *
 * Fill the motors array and create motor control threads.
 */
int init_motors(void)
{
   int rc=0;
   intptr_t i;

   for (i=0; i< NB_MOTORS; i++) {
      sprintf (motors[i].name, "motor %" PRIdPTR, i);
      rc = ed_init (&motors[i].ed, motor_pins[i][IDX_STEP], motor_pins[i][IDX_DIR], STEPS_PR, 10);
      motors[i].ed.speed = MIN_SPEED;

      pthread_mutex_init ( &motors[i].mutex, NULL );
      pthread_cond_init  ( &motors[i].cond,  NULL );
      rc |= pthread_create ( &motors[i].thread, NULL, run_motor, (void*) i);
   }
   return rc;
} // init_motors

/** Close motors and kill threads
 */
void close_motors (void)
{
   int i;

   for (i=0; i<NB_MOTORS; i++) {
      ed_close(&motors[i].ed);

      pthread_cond_destroy (&motors[i].cond);
      pthread_mutex_destroy (&motors[i].mutex);
      pthread_cancel (motors[i].thread);
   }
}//close_motors


/**** MAIN ****/

int main (int argc,char **argv)
{
   int
      connected,
      rc,
      client_port,
      port,
      sock_fd;

   char
      szclient_host[64],
      szclient_ip[64],
      szbuf[BUFSIZ];

   message_t* msg;

   // TODO: better argument handling
   if (argc < 2)
   {
      (void) fprintf(stderr,"usage: %s port\n",argv[0]);
      return(1);
   }
   port=atoi(argv[1]);

   // init of motor control threads
   if ( init_motors() != 0) {
      fprintf(stderr, "cannot init motors\n");
      return 1;
   }

   /* open socket and start listening */
   printf ("Starting server ...\n");
   sock_fd=ServerSocket((u_short) port,5);
   if (sock_fd == -1)
   {
      (void) fprintf(stderr,"Failed to open socket\n");
      return (1);
   }

   /* we will be here only when a client connects */

   /* get client informations */
   rc=getPeerInfo(sock_fd,szclient_host,szclient_ip,(u_short*)&client_port);
   if (rc == 0)
   {
      fprintf(stderr,"================================\n");
      fprintf(stderr,"Client hostname %s\n",szclient_host);
      fprintf(stderr,"Client IP: %s\n",szclient_ip);
      fprintf(stderr,"Client port: %d\n",client_port);
      fprintf(stderr,"================================\n");
   }

   connected=1;
   /* serve the client */
   while (connected)
   {
      // get message header
      rc = sockRead (sock_fd, szbuf, HEADER_SIZE);
      if (rc < 0)
         break;
      if (rc)
      {
         //            printf("ver:%d, size:%d, type:%d\n", szbuf[0], szbuf[1], szbuf[2]);
//          printf ("read %d (exp:%d), still %d bytes to read\n",rc, (int)HEADER_SIZE, (int)(szbuf[1]-HEADER_SIZE));
         // get following part of the message
         rc = sockRead (sock_fd, szbuf+HEADER_SIZE, szbuf[1] - HEADER_SIZE);
         if (rc) {
            msg = (message_t*) szbuf;

            switch (msg->type) {
               case T_DATA_JOY:
               hex_dump_message( (message_t*) szbuf);
//             dump_message( (message_t*) szbuf);
                  if (msg->pl.joystick.x1 != 0) {
                     pthread_mutex_lock ( &motors[0].mutex );
                     // map joystick x1 from 0-32767 to MIN_SPEED-MAX_SPEED
                     motors[0].ed.speed = map (msg->pl.joystick.x1, 0, 32767, MIN_SPEED, MAX_SPEED);
                     if (msg->pl.joystick.x1 > 0) {
                        motors[0].steps = 16;
                     }
                     else if (msg->pl.joystick.x1 < 0) {
                        motors[0].steps = -16;
                     }
                     rc = pthread_cond_signal ( &motors[0].cond );
                     pthread_mutex_unlock ( &motors[0].mutex );
                  }
                  if (msg->pl.joystick.y1 != 0) {
                     pthread_mutex_lock ( &motors[1].mutex );
                     // map joystick y1 from 0-32767 to MIN_SPEED-MAX_SPEED
                     motors[1].ed.speed = map (msg->pl.joystick.y1, 0, 32767, MIN_SPEED, MAX_SPEED);
                     if (msg->pl.joystick.y1 > 0) {
                        motors[1].steps = 16;
                     }
                     else if (msg->pl.joystick.y1 < 0) {
                        motors[1].steps = -16;
                     }
                     rc = pthread_cond_signal ( &motors[1].cond );
                     pthread_mutex_unlock ( &motors[1].mutex );
                  }
                  break;
            }
         }
      }
      //send back answer
      sockPuts(sock_fd,"OK\n");
   }


   fprintf(stderr," <closed>\n\n");
   close(sock_fd);
   close_motors();
   return(0);
}
