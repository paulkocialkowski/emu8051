/*
 * operations.c
 *
 * Copyright (C) 2014 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#if HAVE_CONFIG_H
#  include "config.h"
#endif

#include "common.h"
#include "reg8051.h"
#include "cpu8051.h"
#include "sfr.h"
#include "psw.h"
#include "memory.h"
#include "operations.h"

/*
 * ADD and ADDC function identically except that ADDC adds the value of operand
 * as well as the value of the Carry flag whereas ADD does not add the Carry
 * flag to the result.
 */
int
addition(int dst, int src, int carry)
{
	psw_clr_cy();
	psw_clr_ac();
	psw_clr_ov();

	/*
	 * The Overflow (OV) bit is set if there is a carry-out of bit 6 or
	 * out of bit 7, but not both. In other words, if the addition of the
	 * Accumulator, operand and (in the case of ADDC) the Carry flag
	 * treated as signed values results in a value that is out of the
	 * range of a signed byte (-128 through +127) the Overflow flag is
	 * set. Otherwise, the Overflow flag is cleared.
	 */
	if (dst + src + carry > 0xFF) {
		/* Carry from bit 7. */
		psw_set_cy();

		if (((dst & 0x7F) + (src & 0x7F) + carry) < 0x80)
			psw_set_ov(); /* If no carry from bit 6, set OV. */
	} else if (((dst & 0x7F) + (src & 0x7F) + carry) > 0x7F) {
		/* If no carry from bit 7, but carry from bit 6, set OV. */
		psw_set_ov();
	}

	if (((dst & 0x0F) + (src & 0x0F) + carry) > 0x0F)
		psw_set_ac();

	return dst + src + carry;
}
