/*
 * menu.c
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#include "config.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#ifdef HAVE_LIBREADLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif /* HAVE_LIBREADLINE */

#include "common.h"
#include "cpu8051.h"
#include "reg8051.h"
#include "sfr.h"
#include "memory.h"
#include "gp_timers.h"
#include "options.h"
#include "hexfile.h"
#include "keyboard.h"
#include "scanner.h"
#include "menu.h"

extern struct options_t options;

#define YY_NULL 0

#define MENU_PROMPT "-> "

int
menu_get_input(char *buf, ssize_t size)
{
	char *line;
	int max_len;

	max_len = size - 2;

	if (feof(yyin))
		return YY_NULL;

#ifdef HAVE_LIBREADLINE
	/* Get the input line, and if non-empty, place it in the history. */
	line = readline(MENU_PROMPT);
	if (!line)
		return YY_NULL;

	if (line && *line)
		add_history(line);

	if ((int) strlen(line) > max_len) {
		printf("input line too long");
		free(line);
		return YY_NULL;
	}

	strcpy(buf, line);

	/* Readline never gives you the last '\n', so add it back for Lex. */
	strcat(buf, "\n");

	free(line);
#else
	/* It's okay to print a prompt if we are redirecting stdout,
	 * as long as stdin is still a tty.  Otherwise, don't print
	 * a prompt at all if stdin is redirected.
	 */
	(void) fputs(MENU_PROMPT, stdout);
	(void) fflush(stdout);	/* for svr4 */
	line = fgets(buf, max_len, stdin);
	if (!line)
		return YY_NULL;
#endif

	return strlen(buf);
}

void
menu_display_usage(void)
{
	int id;

	printf("  " PACKAGE_NAME " commands, [] = options:\n"
	       "\n"
	       "  sb [ADDRESS]        Set breakpoint at PC or ADDRESS\n"
	       "  rb [ADDRESS]        Remove breakpoint at PC or ADDRESS\n"
	       "                        ADDRESS = all: clear all breakpoints\n"
	       "  db                  Display breakpoints\n"
	       "  de ADDRESS NUM      Dump NUM bytes from ADDRESS in external data"
	       " memory\n"
	       "  di ADDRESS NUM      Dump NUM bytes from ADDRESS in internal data"
	       " memory\n"
	       "  dp ADDRESS NUM      Dump NUM bytes from ADDRESS in program"
	       " memory\n"
	       "  dr                  Display registers\n"
	       "  h or ?              Display this help menu\n"
	       "  q                   Quit\n"
	       "  r [NUM]             Run until breakpoint or for NUM "
	       "instructions\n"
	       "  s                   Step (execute 1 instruction)\n"
	       "  u [ADDRESS] [NUM]   Unassemble NUM instructions at ADDRESS\n"
	       "  we ADDRESS VAL      Write VAL at ADDRESS in external data memory\n"
	       "  wi ADDRESS VAL      Write VAL at ADDRESS in internal data memory\n"
	       "  wp ADDRESS VAL      Write VAL at ADDRESS in program memory\n"
	       "  wr REGISTER VAL     Write VAL at REGISTER (REGISTER is name of"
	       " register)\n"
	       "  x                   Enable tracing\n"
	       "  X                   Disable tracing\n"
	       "  z                   Reset processor\n"
	       "  zt ID               Reset emulator timer ID (");

	for (id = 0; id < GP_TIMERS_COUNT; id++) {
		printf("%c", 'A' + id);
		if (id < (GP_TIMERS_COUNT - 1))
			printf(", ");
	}
	printf(")\n");
}

/* Disassemble num instructions at address */
void
disassemble_num(unsigned int address, int num)
{
	char str[255];
	int row;

	for (row = 0; row < num; row++) {
		address += cpu8051_disasm(address, str);
		printf("%s\n", str);

		if (address > 0xFFFF)
			return;
	}
}

/* Capitalize all letters in buffer */
static void
uppercase(char *buffer)
{
	size_t k;

	for (k = 0; k < strlen(buffer); k++)
		buffer[k] = toupper(buffer[k]);
}

/* Set NewValue to Register */
void
register_set(char *name, int value)
{
	struct regwin_infos_t *regwin_infos;

	uppercase(name);

	log_debug("  Modify register %s to $%04X", name, value);

	regwin_infos = sfr_get_infos(name);

	if (regwin_infos == NULL) {
		printf("Invalid register name\n");
		return;
	}

	regwin_write(regwin_infos, value);
}

/* CPU reset and Console UI update */
void
console_reset(void)
{
	log_info("Resetting...");
	cpu8051_reset();
	log_info("Done");
}

/*
 * CPU exec and Console UI update
 * num = number of instructions to execute.
 *       set to -1: run to infinity
 */
void
console_exec(int num)
{
	keyboard_init();

	log_info("Program executing...");

	(void) cpu8051_run(num, kbhit);

	if (kbhit()) {
		(void) getch(); /* Flush key */
		log_info("Caught break signal!");
	}

	keyboard_reset();
	console_show_registers();
	disassemble_num(cpu8051.pc, 1);
}

/* CPU trace and Console UI update */
void
console_trace(void)
{
	(void) cpu8051_exec();
	console_show_registers();
	disassemble_num(cpu8051.pc, 1);
}

/* Show CPU registers, one per line */
static void
console_dump_sfr_registers_detailed(void)
{
	int k;

	for (k = 0; k < SFR_REGS; k++) {
		struct regwin_infos_t *regwin_infos;
		int val;

		regwin_infos = sfr_get_infos_from_row(k);

		printf("%s = ", regwin_infos->name);

		val = regwin_read(k);
		if (regwin_infos->w == 2)
			printf("$%02X", val);
		else if (regwin_infos->w == 4)
			printf("$%04X", val);

		printf("\n");
	}
}

/* Show CPU registers, compact format */
static void
console_dump_sfr_registers_compact(void)
{
	int id;
	unsigned char PSW = mem_read_direct(_PSW_, true);
	int bank_sel = (PSW & 0x18);

	printf("----------------------------------------------------------------------\n");
	printf("|  PC  | SP | DPTR | ACC |  B | PSW:  CY  AC  F0 RS1 RS0  OV   -   P |\n");
	printf("| %.4X | %.2X | %.4X |  %.2X | %.2X |", cpu8051.pc,
	       mem_read_direct(_SP_, true), mem_sfr_read_dptr(),
	       mem_read_direct(_ACC_, true), mem_read_direct(_B_, true));
	printf("        %d   %d   %d   %d   %d   %d   %d   %d |",
	       (PSW >> 7) & 1, (PSW >> 6) & 1, (PSW >> 5) & 1, (PSW >> 4) & 1,
	       (PSW >> 3) & 1, (PSW >> 2) & 1, (PSW >> 1) & 1, PSW & 1);
	printf("\n");
	printf("----------------------------------------------------------------------\n");

	printf("| TCON | TMOD | IE | IP | R0 | R1 | R2 | R3 | R4 | R5 | R6 | R7 |    |\n");
	printf("|   %.2X |   %.2X | %.2X | %.2X ", mem_read_direct(_TCON_, true),
	       mem_read_direct(_TMOD_, true), mem_read_direct(_IE_, true), mem_read_direct(_IP_, true));
	printf("| %.2X | %.2X | %.2X | %.2X ",
	       mem_read_direct(bank_sel + _R0_, true),
	       mem_read_direct(bank_sel + _R1_, true),
	       mem_read_direct(bank_sel + _R2_, true),
	       mem_read_direct(bank_sel + _R3_, true));
	printf("| %.2X | %.2X | %.2X | %.2X ",
	       mem_read_direct(bank_sel + _R4_, true),
	       mem_read_direct(bank_sel + _R5_, true),
	       mem_read_direct(bank_sel + _R6_, true),
	       mem_read_direct(bank_sel + _R7_, true));
	printf("|    |\n");
	printf("----------------------------------------------------------------------\n");

	for (id = 0; id < GP_TIMERS_COUNT; id++)
		printf("| Emulator timer %c: %08d |\n", 'A' + id, gp_timer_read(id));

	printf("------------------------------\n");
}

/* Show CPU registers */
void
console_show_registers(void)
{
	if (options.stop_address != 0)
		console_dump_sfr_registers_detailed();
	else
		console_dump_sfr_registers_compact();
}
