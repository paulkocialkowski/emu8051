/*
 * gtksizes.h
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

#ifndef _GTKSIZES_HPP_
#define _GTKSIZES_HPP_

#define NUMBER_OF_BUTTONS  5
#define BUTTON_WIDTH       60
#define BUTTON_HEIGHT      60
#define BUTTONS_BAR_WIDTH  (NUMBER_OF_BUTTONS * BUTTON_WIDTH)
#define BUTTONS_BAR_HEIGHT BUTTON_HEIGHT

#define REG_WIN_WIDTH      100
#define REG_WIN_HEIGHT     390

#define PGM_WIN_WIDTH      480
#define PGM_WIN_HEIGHT     390

#define MEM_WIN_WIDTH      590
#define MEM_WIN_HEIGHT     280

#define MENU_BAR_HEIGHT    0

#define MAIN_WIN_WIDTH     (REG_WIN_WIDTH + PGM_WIN_WIDTH)
#define MAIN_WIN_HEIGHT    (BUTTONS_BAR_HEIGHT + REG_WIN_HEIGHT + MEM_WIN_HEIGHT)

#endif
