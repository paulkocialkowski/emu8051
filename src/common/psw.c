/*
 * psw.c
 *
 * Copyright (C) 2013 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#include <stdbool.h>

#include "common.h"
#include "reg8051.h"
#include "memory.h"

/* Returns 0 or 1 */
int
psw_read_bit(unsigned int bit)
{
	return (mem_read8(SFR_MEM_ID, _PSW_, true) >> bit) & 0x01;
}

void
psw_write_bit(unsigned int bit, int val)
{
	uint8_t psw = mem_read8(SFR_MEM_ID, _PSW_, true);

	if (val)
		psw |= (1 << bit);  /* Set */
	else
		psw &= ~(1 << bit); /* Clear */

	mem_write8(SFR_MEM_ID, _PSW_, psw, true); /* Save updated value */
}

/* Returns 0 or 1 */
int
psw_read_cy(void)
{
	return psw_read_bit(PSW_BIT_CY);
}

void
psw_write_cy(int cy)
{
	psw_write_bit(PSW_BIT_CY, cy);
}

void
psw_set_cy(void)
{
	psw_write_bit(PSW_BIT_CY, 1);
}

void
psw_clr_cy(void)
{
	psw_write_bit(PSW_BIT_CY, 0);
}

/* Returns 0 or 1 */
int
psw_read_ac(void)
{
	return psw_read_bit(PSW_BIT_AC);
}

void
psw_write_ac(int ac)
{
	psw_write_bit(PSW_BIT_AC, ac);
}

void
psw_set_ac(void)
{
	psw_write_bit(PSW_BIT_AC, 1);
}

void
psw_clr_ac(void)
{
	psw_write_bit(PSW_BIT_AC, 0);
}

/* Returns 0 or 1 */
int
psw_read_ov(void)
{
	return psw_read_bit(PSW_BIT_OV);
}

void
psw_write_ov(int ov)
{
	psw_write_bit(PSW_BIT_OV, ov);
}

void
psw_set_ov(void)
{
	psw_write_bit(PSW_BIT_OV, 1);
}

void
psw_clr_ov(void)
{
	psw_write_bit(PSW_BIT_OV, 0);
}

/*
 * Compute parity of bits in accumulator:
 *   parity = 0: even number of ones in accumulator
 *   parity = 1: odd  number of ones in accumulator
 */
void
psw_compute_parity_bit(void)
{
	int parity = 0;
	uint8_t acc = mem_read8(SFR_MEM_ID, _ACC_, true);

	while (acc) {
		parity = !parity;
		acc = acc & (acc - 1);
	}

	psw_write_bit(PSW_BIT_P, parity);
}
