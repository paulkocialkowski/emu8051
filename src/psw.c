/*
 * psw.c
 *
 * Copyright (C) 2013 Hugo Villeneuve <hugo@hugovil.com>
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

#include "common.h"
#include "reg8051.h"
#include "memory.h"

void
psw_write_cy(int cy)
{
	u_int8_t psw = memory_read8(INT_MEM_ID, _PSW_);

	if (cy)
		psw |= PSW_FLAG_CY;  /* Set */
	else
		psw &= ~PSW_FLAG_CY; /* Clear */

	memory_write8(INT_MEM_ID, _PSW_, psw); /* Save updated value */
}

void
psw_set_cy(void)
{
	psw_write_cy(1);
}

void
psw_clr_cy(void)
{
	psw_write_cy(0);
}

/* Returns 0 or 1 */
int
psw_read_cy(void)
{
	return memory_read8(INT_MEM_ID, _PSW_) >> PSW_BIT_CY;
}
