
#ifndef __MAIN_H__
#define __MAIN_H__

#include <pthread.h>
#include "easydriver.h"

#define NB_MOTORS 6

/** define gpio pins for every motors */
char motor_pins[NB_MOTORS][3] = {
   /* {STEP, DIR, MS2} */
   {68, 67, 44}, /* motor0: P8_10, P8_8,  P8_12 */
   {46, 26, 65}, /* motor1; P8_16, P8_14, P8_18 */
   {47, 27, 23}, /* motor2; P8_15, P8_17, P8_13 */
   {69, 45, 66}, /* motor3; P8_9,  P8_11, P8_7  */
   {51, 50,  4}, /* motor4; P9_16, P9_14, P9_18 */
   {15,  2, 14}, /* motor5; P9_24, P9_22, P9_26 */
};
#define IDX_STEP 0
#define IDX_DIR  1
#define IDX_MS2  2

/** Motors parameters */
// steps per revolution
#define STEPS_PR 1600
#define MIN_SPEED 500
#define MAX_SPEED 40000

/** control range parameters */
// joystick
#define MIN_RANGE_JOY_AXIS 0
#define MAX_RANGE_JOY_AXIS 32767
// android
#define MIN_RANGE_AND_PITCH 0
#define MAX_RANGE_AND_PITCH 180
#define MIN_RANGE_AND_ROLL 0
#define MAX_RANGE_AND_ROLL 90
#define MIN_RANGE_AND_AZIM 0
#define MAX_RANGE_AND_AZIM 0

/**
 * Motor structure
 */
typedef struct {
   char name[10];               /**< Motor name */
   easydriver_t ed;             /**< driver structure for control data */
   int steps;                   /**< number of steps to move each time thread is unlocked */

   pthread_t       thread;      /**< the motor control thread */
   pthread_mutex_t mutex;       /**< mutex to access this structure */
   pthread_cond_t  cond;        /**< blocking condition of the thread */
} motor_t;


#endif /* __MAIN_H__ */