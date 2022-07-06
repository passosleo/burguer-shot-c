#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <time.h>
#include <termios.h>
#include <stdio.h>

static struct termios old, current;

void clearScreen ();
void delay();
void initTermios(int);
void resetTermios(void);
char getch_(int);
char getch(void);
char getche(void);

/* Clear terminal screen */
void clearScreen() {
  printf("\033[0;0H\033[2J");
};

/* Delay execution by a given seconds */
void delay(unsigned int secs) {
  unsigned int end = time(0) + secs;
  while (time(0) < end);
};

/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  current = old; /* make new settings same as old settings */
  current.c_lflag &= ~ICANON; /* disable buffered i/o */
  if (echo) {
      current.c_lflag |= ECHO; /* set echo mode */
  } else {
      current.c_lflag &= ~ECHO; /* set no echo mode */
  }
  tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void) 
{
  return getch_(0);
}

/* Read 1 character with echo */
char getche(void) 
{
  return getch_(1);
}

#endif