// Keyboard.hpp

#ifndef _KEYBOARD_HPP_
#define _KEYBOARD_HPP_

#include <termios.h>
#include <unistd.h>

static struct termios orig, newtio;
static int peek = -1;

int kbhit()
{
  char ch;
  int nread;
  if(peek != -1) return 1;
  newtio.c_cc[VMIN]=0;
  tcsetattr(0, TCSANOW, &newtio);
  nread = read(0,&ch,1);
  newtio.c_cc[VMIN]=1;
  tcsetattr(0, TCSANOW, &newtio);
  if(nread == 1) {
   peek = ch;
   return 1;
  }
  return 0;
}

int getch()
{
  char ch;
  if(peek != -1) {
    ch = peek;
    peek = -1;
    return ch;
  }
  read(0,&ch,1);
  return ch;
}

void InitUnixKB( )
{
  tcgetattr(0, &orig);
  newtio = orig;
  newtio.c_lflag &= ~ICANON;
  newtio.c_lflag &= ~ECHO;
  newtio.c_lflag &= ~ISIG;
  newtio.c_cc[VMIN] = 1;
  newtio.c_cc[VTIME] = 0;
  tcsetattr(0, TCSANOW, &newtio);
}

void ResetUnixKB( )
{
  tcsetattr(0,TCSANOW, &orig);
}

#endif

