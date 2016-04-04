/*
 * cpu8051.h
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#ifndef CPU8051_H
#define CPU8051_H 1

#include <stdint.h>

/* Maximum number of BreakPoints */
#define MAXBP 32

struct cpu8051_t {
	unsigned int pc; /* Program counter */
	int interrupt_latency;
	int interrupt_priority;
	int bp_count;
	unsigned int bp[MAXBP]; /* List of breakpoints */
};

/* Exported variables */
#undef _SCOPE_
#ifdef CPU8051_M
#  define _SCOPE_ /**/
#else
#  define _SCOPE_ extern
#endif

_SCOPE_ struct cpu8051_t cpu8051;

int
breakpoint_is_defined(unsigned int address);

void
breakpoints_show(void);

void
breakpoint_set(unsigned int address);

void
breakpoint_clr(unsigned int address);

void
breakpoints_clr_all(void);

void
breakpoint_toggle(unsigned int address);

void
cpu8051_init(void);

int
cpu8051_exec(void);

int
cpu8051_run(int instr_count, int (*interface_stop)(void));

void
cpu8051_reset(void);

int
cpu8051_disasm_mnemonic(unsigned char opcode, char *buf);

void
cpu8051_disasm_args(unsigned int address, char *buf);

int
cpu8051_disasm(unsigned int address, char *text);

#endif /* CPU8051_H */
