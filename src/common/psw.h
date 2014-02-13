/*
 * psw.h
 *
 * Copyright (C) 2013 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#ifndef PSW_H
#define PSW_H 1

int
psw_read_bit(unsigned int bit);

void
psw_write_bit(unsigned int bit, int val);

int
psw_read_cy(void);

void
psw_write_cy(int cy);

void
psw_clr_cy(void);

void
psw_set_cy(void);

int
psw_read_ac(void);

void
psw_write_ac(int ac);

void
psw_clr_ac(void);

void
psw_set_ac(void);

int
psw_read_ov(void);

void
psw_write_ov(int ov);

void
psw_clr_ov(void);

void
psw_set_ov(void);

void
psw_compute_parity_bit(void);

#endif /* PSW_H */
