
#ifndef __MAIN_H__
#define __MAIN_H__

#include <pthread.h>
#include "easydriver.h"

#define NB_MOTORS 6

/** define gpio pins for every motors */
char motor_pins[NB_MOTORS][4] = {
   /* {STEP, DIR, MS2, POSITION} */
   {68, 67, 44, 31}, /* motor0: P8_10, P8_8,  P8_12, P9_13 */
   {46, 26, 65, 48}, /* motor1; P8_16, P8_14, P8_18, P9_15 */
   {47, 27, 23,  5}, /* motor2; P8_15, P8_17, P8_13, P9_17 */
   {69, 45, 66,  3}, /* motor3; P8_9,  P8_11, P8_7 , P9_21 */
   {51, 50,  4, 49}, /* motor4; P9_16, P9_14, P9_18, P9_23 */
   {15,  2, 14, 115}, /* motor5; P9_24, P9_22, P9_26, P9_27 */
};
#define IDX_STEP 0
#define IDX_DIR  1
#define IDX_MS2  2
#define IDX_POS  3

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
#define MAX_RANGE_AND_PITCH 2000
#define MIN_RANGE_AND_ROLL 0
#define MAX_RANGE_AND_ROLL 2000
#define MIN_RANGE_AND_AZIM 0
#define MAX_RANGE_AND_AZIM 2000

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