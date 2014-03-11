#include "easydriver.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <stdio.h>

/* usleep delay between each step */
#define SPEED_TO_uSLEEP(ed) ((60000000/ed->stepsPerRevolution)/ed->speed)

/** driver initialisation
 *
 * @param ed easydriver structure pointer
 * @param gpio_STEP gpio pin for steps (gpio number as seen by linux kernel)
 * @param gpio_DIR gpio pin for direction (gpio number as seen by linux kernel)
 * @param gpio_MS2 gpio pin for MS2 (gpio number as seen by linux kernel)
 * @param gpio_POS gpio pin for position detection (gpio number as seen by linux kernel)
 * @param stepsPerRevolution number of steps for one motor revolution
 * @param speed speed of rotation in RPM
 */
int ed_init (easydriver_t* ed, char gpio_STEP, char gpio_DIR, char gpio_MS2, char gpio_POS, int stepsPerRevolution, float speed)
{
   // init struct easydriver_t
   memset (ed, 0, sizeof(easydriver_t));
   ed->stepsPerRevolution = stepsPerRevolution;
   ed->speed = speed;

   // init gpio
   gpio_export (&ed->gpio_STEP, gpio_STEP);
   gpio_export (&ed->gpio_DIR, gpio_DIR);
   gpio_export (&ed->gpio_MS2, gpio_MS2);
   gpio_export (&ed->gpio_POS, gpio_POS);

   gpio_set_dir ( &ed->gpio_DIR, OUTPUT_PIN);
   gpio_set_dir ( &ed->gpio_STEP,  OUTPUT_PIN);
   gpio_set_dir ( &ed->gpio_MS2,  OUTPUT_PIN);
   gpio_set_dir ( &ed->gpio_POS,  INPUT_PIN);//read input pin

   gpio_set_edge(&ed->gpio_POS, "rising");//damien

   gpio_set_value ( &ed->gpio_DIR, ed->direction);
   ed_precision ( ed, FULL );

   return 0;
} // ed_init


/** poll initialisation
 *
 * @param ed easydriver structure pointer
 * @return -1 (error), 0 (timeout), 1(risingedge)
 *
 */

int ed_poll(easydriver_t* ed){//damien

   struct pollfd fdset[1];
   int nfds = 1;
   int gpio_fd, timeout, rc;
   unsigned int gpio;
   char *buf[2];
   int etat_gpio;

   gpio = atoi(&ed->gpio_POS.num);
   gpio_fd = gpio_fd_open(gpio);
//  gpio_fd = gpio_fd_open_g(&ed->gpio_MS2);

   timeout = POLL_TIMEOUT;//check time_out value

   memset((void*)fdset, 0, sizeof(fdset));

   fdset[0].fd = gpio_fd;
   fdset[0].events = POLLPRI;

   rc = poll(fdset, nfds, timeout); //bloquant, fin du poll() a timeout

   if (rc < 0) {//poll() failed
      fprintf(stderr,"poll() failed for gpio %d\n", gpio);
      gpio_fd_close(gpio_fd);
      return -1;
   }

   if (rc == 0) {//out of time
      if (read(gpio_fd, &buf,2) != 2) {
         perror("read");
         return -1;
      }
      etat_gpio = atoi(buf[0]);
      gpio_fd_close(gpio_fd);
      // verifier la valeur sortie du capteur
      if (etat_gpio == 0){//etat bas, moteur dans la plage
         return 0;
	  }
      else {//etat haut, moteur en fin de course
         return 1;
	  }
   }

   if (fdset[0].revents & POLLPRI){// finDeCourse
      gpio_fd_close(gpio_fd);
      return 1;
   }

   else{
//	   fprintf(stderr,"poll() filed\n");
      gpio_fd_close(gpio_fd);
      return -1;
   }

}//ed_poll


/** rotate a arbitrary number of steps
 *
 * @param ed easydriver structure pointer
 * @param numberOfSteps steps to rotate. >0: clockwise, <0:anticlockwise
 */
 /*
int ed_step(easydriver_t* ed, int numberOfSteps)
{
   int i=0;

   if(numberOfSteps>=0) {
      gpio_set_value(&ed->gpio_DIR, CLOCKWISE);
   }
   else
   {
      gpio_set_value(&ed->gpio_DIR, ANTICLOCKWISE);
      numberOfSteps *= -1;
   }

   for(i=0; i<numberOfSteps; i++){
      gpio_set_value(&ed->gpio_STEP, LOW);
      usleep(1000);
      gpio_set_value(&ed->gpio_STEP, HIGH);
      usleep(1000);
      usleep( SPEED_TO_uSLEEP(ed) );
   }
   return 0;
} // ed_step
* */

int ed_step(easydriver_t* ed, int numberOfSteps)
{
   int i = 0;
   int sensMot = 0;
   int finDeCourse = 0;


   sensMot = numberOfSteps;

   if(numberOfSteps<=0) {
      numberOfSteps *= -1;
   }

   for(i=0; i<numberOfSteps; i++){

      finDeCourse = ed_poll(ed);//butee = 1

      if(sensMot>=0 && finDeCourse == 0) {//keep motor direction
      gpio_set_value(&ed->gpio_DIR, CLOCKWISE);
      }
      else if(sensMot>=0 && finDeCourse == 1) {//change motor direction
         gpio_set_value(&ed->gpio_DIR, ANTICLOCKWISE);
      }
      else if (sensMot<=0 && finDeCourse == 0) {//keep motor direction
         gpio_set_value(&ed->gpio_DIR, ANTICLOCKWISE);
      }
      else   {//change motor direction
         gpio_set_value(&ed->gpio_DIR, CLOCKWISE);
      }

      gpio_set_value(&ed->gpio_STEP, LOW);
      usleep(1000);
      gpio_set_value(&ed->gpio_STEP, HIGH);
      usleep(1000);
      usleep( SPEED_TO_uSLEEP(ed) );
   }
   return 0;
} // ed_step



/** set motor step precision
 *
 * After, each step will be a full step or a 1/4 step.
 *
 * @param ed easydriver structure pointer
 * @param precision FULL or QUATER
 */
int ed_precision (easydriver_t* ed, char precision)
{
   gpio_set_value (&ed->gpio_MS2, precision);
   return 0;
} // ed_precision

/** Close the easydriver
 * @param ed  easydriver structure pointer
 */
int ed_close (easydriver_t* ed)
{
   gpio_unexport (&ed->gpio_DIR);
   gpio_unexport (&ed->gpio_STEP);
   return 0;
} // ed_close
