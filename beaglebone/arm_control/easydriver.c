#include "easydriver.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* usleep delay between each step */
#define SPEED_TO_uSLEEP(ed) ((60000000/ed->stepsPerRevolution)/ed->speed)

int ed_init (easydriver_t* ed, char gpio_STEP, char gpio_DIR, int stepsPerRevolution, float speed)
{
   // init struct easydriver_t
   memset (ed, 0, sizeof(easydriver_t));
   ed->stepsPerRevolution = stepsPerRevolution;
   ed->speed = speed;

   // init gpio
   gpio_export (&ed->gpio_STEP, gpio_STEP);
   gpio_export (&ed->gpio_DIR, gpio_DIR);

   gpio_set_dir ( &ed->gpio_DIR, OUTPUT_PIN);
   gpio_set_dir ( &ed->gpio_STEP,  OUTPUT_PIN);
   gpio_set_value ( &ed->gpio_DIR, ed->direction);

   return 0;
} // ed_init


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

int ed_close (easydriver_t* ed)
{
   gpio_unexport (&ed->gpio_DIR);
   gpio_unexport (&ed->gpio_STEP);
   return 0;
} // ed_close
