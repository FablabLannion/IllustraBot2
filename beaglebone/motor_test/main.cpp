#include <ncurses.h>
#include <unistd.h>

#define MAX_STEPS 240
#define BIG_INC 10

void print_help(void) {
   move (0, 0);
   printw("Help\n");
   printw("  v  / ^  : number of steps\n");
   printw("  -> / <- : rotate\n");
   printw("  q       : quit\n");
}

void print_steps ( int steps ) {
   move (5, 1);
   printw ("Steps per stroke : %03d", steps);
}

void print_doing ( bool dir, int steps) {
   move (7,1);
   printw ("Going %s for %03d steps ...", (dir)?"right":"left", steps);
}

int main()
{
   int ch;
   int steps = 1;
   bool exit = FALSE; 
   
   initscr();                 /* Start curses mode     */
   noecho();                  /* supress echoing */
   curs_set(0);               /* hide cursor */
   keypad(stdscr, TRUE);      /* get special keys ie:arrows */

   
   while ( ! exit ) {
      clear();
      print_help();
      print_steps (steps);
      refresh();                 /* Print it on to the real screen */
      
      ch = getch();                   /* Wait for user input */
      switch (ch) {
         case KEY_DOWN:
            (steps <= 1)?steps = MAX_STEPS:steps--;
            break;
         case KEY_UP:
            (steps >= MAX_STEPS)? steps = 1:steps++;
            break;
         case KEY_PPAGE:
            steps += BIG_INC;
            if (steps > MAX_STEPS) steps = 1+(steps%MAX_STEPS);
            break;
         case KEY_NPAGE:
            steps -= BIG_INC;
            if (steps <= 1) steps = MAX_STEPS+steps;
            break;
         case KEY_RIGHT:
            print_doing (TRUE, steps);
            refresh();                 /* Print it on to the real screen */
            sleep(1);
            break;
         case KEY_LEFT:
            print_doing (FALSE, steps);
            refresh();                 /* Print it on to the real screen */
            sleep(1);
            break;
         case 'q':
            exit = TRUE;
            break;
      }
   }
   endwin();                  /* End curses mode    */

   return 0;
}
