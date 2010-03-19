/*
 * keyboard.c
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <termios.h>
#include <unistd.h>

static struct termios orig, newtio;
static int peek = -1;

int
kbhit(void)
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

int
getch(void)
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

void
InitUnixKB(void)
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

void
ResetUnixKB(void)
{
	tcsetattr(0,TCSANOW, &orig);
}
