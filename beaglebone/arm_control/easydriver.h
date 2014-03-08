#include "SimpleGPIO.h"

#ifndef __EASYDRIVER_H__
#define __EASYDRIVER_H__

// defines for motor direction
#define CLOCKWISE     LOW
#define ANTICLOCKWISE HIGH

// defines for motor precision
#define FULL    LOW
#define QUATER  HIGH

typedef struct {
   gpio_t gpio_STEP;           /**< GPIO for step pin */
   gpio_t gpio_DIR;            /**< GPIO for direction pin */
   gpio_t gpio_MS2;            /**< GPIO for MS2 pin */
   int stepsPerRevolution;     /**< motor number of steps in one revolution */
   int speed;                  /**< speed in RPM */
   char direction;             /**< motor direction */
} easydriver_t;

int ed_init (easydriver_t* ed, char gpio_STEP, char gpio_DIR, char gpio_MS2, int stepsPerRevolution, float speed);
int ed_close (easydriver_t* ed);

int ed_poll(easydriver_t* ed);//damien
int ed_step (easydriver_t* ed, int numberOfSteps);
int ed_precision (easydriver_t* ed, char precision);

#endif /* __EASYDRIVER_H__ */
