#include <ncurses.h>
#include <unistd.h>

#define MAX_STEPS 999
#define BIG_INC 10

void print_help(WINDOW*win) {
   wmove (win, 1, 1);
   mvwprintw(win, 6, 1, "Help");
   mvwprintw(win, 7, 1, "  0-9,up,dwn  : number of steps");
   mvwprintw(win, 8, 1, "  ->, <-      : rotate");
   mvwprintw(win, 9, 1, "  q           : quit");
}

void print_steps (WINDOW* win, int steps ) {
   mvwprintw (win, 2, 3, "Steps per stroke : %03d", steps);
}

void print_doing (WINDOW* win,  bool dir, int steps) {
   mvwprintw (win, 4, 3, "Going %s for %03d steps ...", (dir)?"right":"left", steps);
}

int main()
{
   int ch;
   int steps = 1;
   bool exit = FALSE; 
   WINDOW * win;
   
   initscr();                 /* Start curses mode     */
   noecho();                  /* supress echoing */
   curs_set(0);               /* hide cursor */
   keypad(stdscr, TRUE);      /* get special keys ie:arrows */
   win = newwin(11, 35, 1, 0);
   mvprintw (0, 9, "== Motor Test ==");

   
   while ( ! exit ) {
      werase(win);
      wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
      print_help(win);
      print_steps (win, steps);
      refresh();
      wrefresh(win);
      
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
            if (steps > MAX_STEPS) steps = steps%MAX_STEPS;
            break;
         case KEY_NPAGE:
            steps -= BIG_INC;
            if (steps < 1) steps = MAX_STEPS+steps;
            break;
         case KEY_RIGHT:
            print_doing (win, TRUE, steps);
            wrefresh(win);                 /* Print it on to the real screen */
            sleep(1);
            break;
         case KEY_LEFT:
            print_doing (win, FALSE, steps);
            wrefresh(win);                 /* Print it on to the real screen */
            sleep(1);
            break;
         case '0':
         case '1':
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
         case '7':
         case '8':
         case '9':
            steps = steps*10 + ch - '0';
            steps = steps - ( (steps / 1000) * 1000 );
            break;
         case 'q':
            exit = TRUE;
            break;
      }
   }
   endwin();                  /* End curses mode    */

   return 0;
}
