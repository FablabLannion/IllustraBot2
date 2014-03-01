#include "easydriver.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* usleep delay between each step */
#define SPEED_TO_uSLEEP(ed) ((60000000/ed->stepsPerRevolution)/ed->speed)

/** driver initialisation
 *
 * @param ed easydriver structure pointer
 * @param gpio_STEP gpio pin for steps (gpio number as seen by linux kernel)
 * @param gpio_DIR gpio pin for direction (gpio number as seen by linux kernel)
 * @param gpio_MS2 gpio pin for MS2 (gpio number as seen by linux kernel)
 * @param stepsPerRevolution number of steps for one motor revolution
 * @param speed speed of rotation in RPM
 */
int ed_init (easydriver_t* ed, char gpio_STEP, char gpio_DIR, char gpio_MS2, int stepsPerRevolution, float speed)
{
   // init struct easydriver_t
   memset (ed, 0, sizeof(easydriver_t));
   ed->stepsPerRevolution = stepsPerRevolution;
   ed->speed = speed;

   // init gpio
   gpio_export (&ed->gpio_STEP, gpio_STEP);
   gpio_export (&ed->gpio_DIR, gpio_DIR);
   gpio_export (&ed->gpio_MS2, gpio_MS2);

   gpio_set_dir ( &ed->gpio_DIR, OUTPUT_PIN);
   gpio_set_dir ( &ed->gpio_STEP,  OUTPUT_PIN);
   gpio_set_dir ( &ed->gpio_MS2,  OUTPUT_PIN);

   gpio_set_value ( &ed->gpio_DIR, ed->direction);
   ed_precision ( ed, FULL );

   return 0;
} // ed_init

/** rotate a arbitrary number of steps
 *
 * @param ed easydriver structure pointer
 * @param numberOfSteps steps to rotate. >0: clockwise, <0:anticlockwise
 */
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
//       usleep(1);
      gpio_set_value(&ed->gpio_STEP, HIGH);
//       usleep(1);
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
