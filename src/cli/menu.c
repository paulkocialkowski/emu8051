/*
 * menu.c
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
#include "timers.h"
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
	printf("      *******************\n"
	       "      *  8051 Emulator  *\n"
	       "      *******************\n"
	       "\n"
	       "      Available commands, [ ] = options\n"
	       "\n"
	       "  Set Breakpoint.............. SB [address]\n"
	       "  Remove Breakpoint........... RB [address]\n"
	       "                                  address = all:"
	       " clear all breakpoints\n"
	       "  Display Breakpoint(s)....... DB\n"
	       "  Dump External Data Memory... DE [address] [size]\n"
	       "  Dump Internal Data Memory... DI [address] [size]\n"
	       "  Dump Program Memory......... DP [address] [size]\n"
	       "  Display Registers........... DR\n"
	       "  Help........................ H or ?\n"
	       "  Modify External Data Memory. ME address value\n"
	       "  Modify Internal Data Memory. MI address value\n"
	       "  Modify Program Memory....... MP address value\n"
	       "  Modify Register............. MR register value\n"
	       "  Quit........................ Q\n"
	       "  Run......................... R [number of instructions]\n"
	       "  Step........................ S\n"
	       "  Unassemble.................. U [address]"
	       " [number of instructions]\n"
	       "  Reset processor............. Z\n"
	       "  Reset general-purpose timer. ZT\n");
}

/* Disassemble NumberInst instructions at Address */
void
DisasmN(unsigned int Address, int NumberInst)
{
	char TextTmp[255];
	int Row;

	for (Row = 0; Row < NumberInst ; Row++) {
		Address += cpu8051_Disasm(Address, TextTmp);
		printf("%s\n", TextTmp);

		if (Address > 0xFFFF)
			return;
	}
}

/* Capitalize all letters in buffer */
static void
Capitalize(char *buffer)
{
	size_t k;

	for (k = 0; k < strlen(buffer); k++)
		buffer[k] = toupper(buffer[k]);
}

/* Set NewValue to Register */
void
SetRegister(char *register_name, int new)
{
	struct regwin_infos_t *regwin_infos;

	Capitalize(register_name);

	log_debug("  Modify register %s to $%04X", register_name, new);

	regwin_infos = sfr_get_infos(register_name);

	if (regwin_infos == NULL) {
		printf("Invalid register name\n");
		return;
	}

	regwin_write(regwin_infos, new);
}

/* CPU reset and Console UI update */
void
console_reset(void)
{
	log_info("Resetting...");
	cpu8051_Reset();
	log_info("Done");
}

/*
 * CPU exec and Console UI update
 * NbInst = -1: run to infinity
 */
void
console_exec(int NbInst)
{
	InitUnixKB();

	log_info("Program executing...");

	cpu8051_run(NbInst, kbhit);

	if (kbhit()) {
		(void) getch(); /* Flush key */
		log_info("Caught break signal!");
	}

	ResetUnixKB();
	console_show_registers();
	DisasmN(cpu8051.pc, 1);
}

/* CPU trace and Console UI update */
void
console_trace(void)
{
	cpu8051_Exec();
	console_show_registers();
	DisasmN(cpu8051.pc, 1);
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
	unsigned char PSW = cpu8051_ReadD(_PSW_);
	int BankSelect = (PSW & 0x18);

	printf("---------------------------------------------------------------"
	       "-------\n");
	printf("|  PC  | SP | DPTR | ACC |  B | PSW:  CY  AC  F0 RS1 RS0  OV"
	       "   -   P |\n");
	printf("| %.4X | %.2X | %.4X |  %.2X | %.2X |", cpu8051.pc,
	       cpu8051_ReadD(_SP_),
	       memory_sfr_read_dptr(),
	       cpu8051_ReadD(_ACC_), cpu8051_ReadD(_B_));
	printf("        %d   %d   %d   %d   %d   %d   %d   %d |",
	       (PSW >> 7) & 1, (PSW >> 6) & 1, (PSW >> 5) & 1, (PSW >> 4) & 1,
	       (PSW >> 3) & 1, (PSW >> 2) & 1, (PSW >> 1) & 1, PSW & 1);
	printf("\n");
	printf("---------------------------------------------------------------"
	       "-------\n");

	printf("| TCON | TMOD | IE | IP | R0 | R1 | R2 | R3 | R4 | R5 | R6 | R7"
	       " |    |\n");
	printf("|   %.2X |   %.2X | %.2X | %.2X ", cpu8051_ReadD(_TCON_),
	       cpu8051_ReadD(_TMOD_), cpu8051_ReadD(_IE_), cpu8051_ReadD(_IP_));
	printf("| %.2X | %.2X | %.2X | %.2X ",
	       cpu8051_ReadD(BankSelect + _R0_),
	       cpu8051_ReadD(BankSelect + _R1_),
	       cpu8051_ReadD(BankSelect + _R2_),
	       cpu8051_ReadD(BankSelect + _R3_));
	printf("| %.2X | %.2X | %.2X | %.2X ",
	       cpu8051_ReadD(BankSelect + _R4_),
	       cpu8051_ReadD(BankSelect + _R5_),
	       cpu8051_ReadD(BankSelect + _R6_),
	       cpu8051_ReadD(BankSelect + _R7_));
	printf("|    |\n");
	printf("---------------------------------------------------------------"
	       "-------\n");

	printf("| General-purpose Timer: %08d |\n", gp_timer_read());
	printf("-----------------------------------\n");
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
