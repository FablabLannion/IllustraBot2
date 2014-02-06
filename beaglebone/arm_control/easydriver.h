#include "SimpleGPIO.h"

#ifndef __EASYDRIVER_H__
#define __EASYDRIVER_H__

// defines for motor direction
#define CLOCKWISE     LOW
#define ANTICLOCKWISE HIGH

typedef struct {
   gpio_t gpio_STEP;           /**< GPIO for step pin */
   gpio_t gpio_DIR;            /**< GPIO for direction pin */
   int stepsPerRevolution;     /**< motor number of steps in one revolution */
   int speed;                  /**< speed in RPM */
   char direction;             /**< motor direction */
} easydriver_t;

int ed_init (easydriver_t* ed, char gpio_STEP, char gpio_DIR, int stepsPerRevolution, float speed);
int ed_close (easydriver_t* ed);

int ed_step (easydriver_t* ed, int numberOfSteps);

#endif /* __EASYDRIVER_H__ */
