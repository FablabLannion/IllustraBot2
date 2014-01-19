/**
 * test joystick
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
 * compile with :
 *  gcc -c joystick.c
 *  gcc -o joystick_test joystick_test.c joystick.o
 */

#include "joystick.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
   int x;    
   joy_t joy;
   
   if ( joy_init(&joy) != 0) {
      printf("cannot find joystick\n");
      return 1;
   }
   
   printf( " Joystick detected : %s \n \t %d axis \n\t %d buttons \n\n" ,joy.name, joy.num_of_axis , joy.num_of_buttons);
   
   //     fcntl( joy_fd, F_SETFL , O_NONBLOCK ); // use non - blocking methods
   
   while(1) // infinite loop
   {
      // read the joystick 
      joy_read(&joy);
      // print the results
      printf( " X:%6d y:%6d", joy.axis[0] , joy.axis[1]);
      if( joy.num_of_axis > 2)
         printf( " z:%6d" , joy.axis[2] );
      if( joy.num_of_axis > 3)
         printf( " R:%6d" , joy.axis [3]);
      for( x=0 ; x<joy.num_of_buttons ; ++x)
         printf( " B%d:%d" , x, joy.button [x]);
      printf("\n");
      fflush(stdout);
   }
   
   joy_close(&joy);
   
   return 0;
}
