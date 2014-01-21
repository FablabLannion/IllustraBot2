/**
 * Joystick small library
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
 */

#include "joystick.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/** Joystick initialisation
 *
 * Some memory is allocated by this function.
 * Do not forget to call joy_close()
 *
 * @param joy joystick to be used
 * @return 0 in case of success, -1 otherwise
 */
int joy_init (joy_t* joy)
{
   memset (joy, 0, sizeof(joy_t));
   if((joy->joy_fd = open(JOY_DEV,O_RDONLY)) == -1 )
   {
//       printf(" couldn't open the joystick \n " );
      return -1;
   }

   ioctl(joy->joy_fd, JSIOCGAXES , &joy->num_of_axis);
   ioctl(joy->joy_fd, JSIOCGBUTTONS , &joy->num_of_buttons);
   ioctl(joy->joy_fd, JSIOCGNAME(80), &joy->name);

   joy->axis = (int *) calloc(joy->num_of_axis, sizeof(int));
   joy->button = (char *) calloc(joy->num_of_buttons, sizeof (char));
   return 0;
} // joy_init

/** read data from joystick
 *
 * This is a blocking read. So you'll get back from this function
 * only when joystick have moved.
 * To change this behaviour, use joy_nonblocking()
 *
 * @param joy the joystick. It will be updated with read data.
 * @return 0
 */
int joy_read(joy_t* joy)
{
   struct js_event js;

   read (joy->joy_fd, &js , sizeof(struct js_event));
   // see what to do with the event
   switch(js.type & ~ JS_EVENT_INIT)
   {
      case JS_EVENT_AXIS :
            joy->axis [ js.number ] = js.value;
      case JS_EVENT_BUTTON :
            joy->button [js.number ] = js.value;
   }
   return 0;
} // joy_read

/** set joy_read non blocking
 * @param joy joystick
 */
void joy_nonblocking(joy_t* joy)
{
   fcntl( joy->joy_fd, F_SETFL , O_NONBLOCK ); // use non - blocking methods
}

/** close the joystick
 * free allocated memory from joy_init()
 * @param joy the joystick
 */
void  joy_close(joy_t* joy)
{
   free(joy->axis);
   free(joy->button);
   close(joy->joy_fd);
}
