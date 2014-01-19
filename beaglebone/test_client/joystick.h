
#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include <sys/ioctl.h>
#include <linux/joystick.h>

#define JOY_DEV "/dev/input/js0"

/** joystick representation
 * 
 * Must be created by the caller.
 * initialized by joy_init()
 */
typedef struct {
   int joy_fd;          /**< internal joystick file descriptor */
   char name[80];       /**< name of the joystick */
   int* axis;           /**< axis values. length depend on num_of_axis */
   char* button;        /**< buttons value. length depends on num_of_buttons */
   int num_of_axis;     /**< number of axis the joystick has */
   int num_of_buttons;  /**< number of buttons the joystick has */
} joy_t;

int joy_init (joy_t* joy);
int joy_read (joy_t* joy);
void joy_close(joy_t* joy);

#endif /* __JOYSTICK_H__ */
