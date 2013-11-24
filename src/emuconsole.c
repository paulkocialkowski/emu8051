/*
 * emuconsole.c
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

#define _GNU_SOURCE /* For getline() */
#include <stdio.h>
#include <string.h>
#include <ctype.h> /* For isblank, toupper() */
#include "config.h"

#include "common.h"
#include "cpu8051.h"
#include "reg8051.h"
#include "sfr.h"
#include "memory.h"
#include "options.h"
#include "hexfile.h"
#include "keyboard.h"

extern struct options_t options;

/* Capitalize all letters in buffer */
static void
Capitalize(char *buffer)
{
	int k;

	for (k = 0; k < strlen(buffer); k++)
		buffer[k] = toupper(buffer[k]);
}

/* Remove leading spaces from string in buffer */
static void
RemoveSpaces(char *buffer)
{
	int k = 0;

	while ((k < strlen(buffer)) && isblank(buffer[k]))
		k++;

	if (k != 0)
		strcpy(buffer, &buffer[k]);
}

/* CPU exec and Console UI update */
static void
console_exec(char *Address, char *NumberInst)
{
	int NbInst = -1; /* -1 is infinity */
	if (strlen(Address) == 0) {
		log_err("Invalid address");
		return;
	}

	if (STREQ(Address, "PC"))
		cpu8051.pc = Ascii2Hex(Address, strlen(Address));

	if (NumberInst)
		if (strlen(NumberInst) != 0)
			NbInst = Ascii2Hex(NumberInst, strlen(NumberInst));

	InitUnixKB();

	log_info("Program executing...");

	do {
		cpu8051_Exec();
		if (NbInst > 0)
			NbInst--;
	} while (!IsBreakpoint(cpu8051.pc) && !IsStoppoint(cpu8051.pc) &&
		 (NbInst != 0) && !kbhit());
	if (kbhit()) {
		(void) getch(); /* Flush key */
		log_info("Caught break signal!");
	}
	if (NbInst == 0)
		log_info("Number of instructions reached! Stopping!");
	if (IsBreakpoint(cpu8051.pc))
		log_info("Breakpoint hit at %.4X! Stopping!", cpu8051.pc);
	if (IsStoppoint(cpu8051.pc))
		log_info("Stoppoint hit at %.4X! Stopping!", cpu8051.pc);

	ResetUnixKB();
}

/* Disassemble NumberInst instructions at Address */
static void
DisasmN(unsigned int Address, int NumberInst)
{
	char TextTmp[255];
	int Row;

	for (Row = 0; Row < NumberInst ; Row++) {
		Address += cpu8051_Disasm(Address, TextTmp);
		printf("%s\n", TextTmp);
	}
}

/* Disassemble 16 instructions at Address */
static void
Disasm(char *Address, char *NumberInst)
{
	unsigned int MemAddress, NbInst;

	if ((strlen(Address) == 0) || (STREQ(Address, "PC")))
		MemAddress = cpu8051.pc;
	else
		MemAddress = Ascii2Hex(Address, strlen(Address));

	if (strlen(NumberInst) == 0)
		NumberInst = "10";
	NbInst = Ascii2Hex(NumberInst, strlen(NumberInst));
	DisasmN(MemAddress, NbInst);
}

/* Set NewValue to Register */
static void
SetRegister(char *Register, char *NewValue)
{
	if (STREQ(Register, "PC"))
		cpu8051.pc = Ascii2Hex(NewValue, 4);
	else if (STREQ(Register, "A"))
		cpu8051_WriteD(_ACC_, Ascii2Hex(NewValue, 2));
	else if (STREQ(Register, "B"))
		cpu8051_WriteD(_B_, Ascii2Hex(NewValue, 2));
	else if (STREQ(Register, "SP"))
		cpu8051_WriteD(_SP_, Ascii2Hex(NewValue, 2));
	else {
		printf("\nInvalid register name!\n");
		printf("Valid registers are A, B, PC and SP.\n");
	}
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
}

/* Show CPU registers */
static void
console_show_registers(void)
{
	if (options.stop_address != 0)
		console_dump_sfr_registers_detailed();
	else
		console_dump_sfr_registers_compact();
}

/* CPU reset and Console UI update */
static void
console_reset(void)
{
	log_info("Resetting...");
	cpu8051_Reset();
	log_info("Done");
}

/* CPU trace and Console UI update */
static void
console_trace(char *Address)
{
	if (strlen(Address) != 0)
		cpu8051.pc = Ascii2Hex(Address, strlen(Address));
	cpu8051_Exec();
	console_show_registers();
	DisasmN(cpu8051.pc, 1);
}

/* EmuConsole main loop */
static void
console_main(void)
{
	unsigned int Index;
	char *line = NULL;
	int QuitRequest = 0;
	char prompt[] = "-> ";

	char *Title[] = { "      *******************",
			  "      *  8051 Emulator  *",
			  "      *******************",
			  "", 0 };

	char *Menu[] = {
		"      Available commands, [ ] = options",
		"",
		"  Set Breakpoint.............. SB [address]",
		"  Remove Breakpoint........... RB [address]",
		"  Display Breakpoint(s)....... DB",
		"  Dump External Data Memory... DE [address] [size]",
		"  Dump Internal Data Memory... DI [address] [size]",
		"  Dump Program Memory......... DP [address] [size]",
		"  Display Registers content... DR",
		"  Execute..................... EM [address"
		" [number of instructions]]",
		"  Help........................ H",
		"  Modify External Data Memory. ME address value",
		"  Modify Internal Data Memory. MI address value",
		"  Modify Program Memory....... MP address value",
		"  Modify Register............. MR register value",
		"  Quit Emulator............... Q",
		"  Trace mode.................. T [address]",
		"  Unassemble.................. U [address]"
		" [number of instructions]",
		"  Reset processor............. Z", 0 };

	console_reset();

	if (options.stop_address != 0) {
		/* Automatically run program and stop at specified address. */
		console_exec("0x0000", NULL);
		console_show_registers();
		QuitRequest = 1;
	} else {
		Index = 0;
		while (Title[Index] != 0)
			printf("%s\n", Title[Index++]);

		Index = 0;
		while (Menu[Index] != 0)
			printf("%s\n", Menu[Index++]);

		console_show_registers();
	}

	while (!QuitRequest) {
		int slen;
		size_t len = 0;
		char Command[256];
		char Args[256];
		char Parameter1[256];
		char Parameter2[256];

		Parameter1[0] = '\0';
		Parameter2[0] = '\0';

		printf(prompt);
		(void) getline(&line, &len, stdin);
		Capitalize(line);
		RemoveSpaces(line);

		/* Strip trailing newline */
		slen = strlen(line);
		if (line[slen - 1] == '\n')
			line[slen - 1] = '\0';

		/* Find command-arguments delimiter */
		for (Index = 0; Index < strlen(line); Index++) {
			if (isblank(line[Index]))
				break;
		}

		/* Keep only the Command part from the input line */
		memcpy(Command, &line[0], Index);
		Command[Index] = '\0';

		/* Keep only the arguments part from the input line */
		if (Index < strlen(line)) {
			slen = strlen(line) - Index;
			memcpy(Args, &line[Index + 1], slen);
		} else {
			slen = 0;
		}
		Args[slen] = '\0';
		RemoveSpaces(Args);

		/* Find multi-arguments delimiter */
		for (Index = 0; Index < strlen(Args); Index++) {
			if (isblank(Args[Index]))
				break;
		}

		memcpy(Parameter1, &Args[0], Index);
		Parameter1[Index] = '\0';

		if (Index < strlen(Args)) {
			slen = strlen(Args) - Index;
			memcpy(Parameter2, &Args[Index + 1], slen);
		} else {
			slen = 0;
		}
		Parameter2[slen] = '\0';
		RemoveSpaces(Parameter2);

		if (strlen(Command) == 0) {
			goto syntax_error;
			continue;
		}

		if ((strlen(Parameter1) > 4) || (strlen(Parameter2) > 4)) {
			printf("Invalid Parameter Format!\n");
			continue;
		}

		switch (Command[0]) {
		case 'D':
			if (STREQ(Command, "DB") &&
			    (strlen(Parameter1) == 0))
				ShowBreakpoints();
			else if (STREQ(Command, "DE"))
				DumpMem(Parameter1, Parameter2, EXT_MEM_ID);
			else if (STREQ(Command, "DI"))
				DumpMem(Parameter1, Parameter2, INT_MEM_ID);
			else if (STREQ(Command, "DP")) {
				if ((strlen(Parameter1) == 0))
					strcpy(Parameter1, "PC");
				DumpMem(Parameter1, Parameter2, PGM_MEM_ID);
			} else if (STREQ(Command, "DR") &&
				   (strlen(Parameter1) == 0))
				console_show_registers();
			else
				goto syntax_error;
			break;
		case 'E':
			if (STREQ(Command, "EM"))
				console_exec(Parameter1, Parameter2);
			else
				goto syntax_error;
			break;
		case 'H':
			if (STREQ(Command, "H") && (strlen(Parameter1) == 0) &&
			    (strlen(Parameter2) == 0)) {
				Index = 0;
				while (Menu[Index] != 0)
					printf("%s\n", Menu[Index++]);
			} else
				goto syntax_error;
			break;
		case 'M':
			if ((strlen(Parameter1) == 0) ||
			    (strlen(Parameter2) == 0))
				printf("Missing Parameter!\n");
			else if (STREQ(Command, "ME")) {
				unsigned int adresse = Ascii2Hex(Parameter1, 4);
				unsigned char valeur = Ascii2Hex(Parameter2, 2);
				memory_write8(EXT_MEM_ID, adresse, valeur);
			} else if (STREQ(Command, "MI")) {
				unsigned int adresse = Ascii2Hex(Parameter1, 2);
				unsigned char valeur = Ascii2Hex(Parameter2, 2);
				memory_write8(INT_MEM_ID, adresse, valeur);
			} else if (STREQ(Command, "MP")) {
				unsigned int adresse = Ascii2Hex(Parameter1, 4);
				unsigned char valeur = Ascii2Hex(Parameter2, 2);
				memory_write8(PGM_MEM_ID, adresse, valeur);
			} else if (STREQ(Command, "MR"))
				SetRegister(Parameter1, Parameter2);
			else
				goto syntax_error;
			break;
		case 'Q':
			if (STREQ(Command, "Q") && (strlen(Parameter1) == 0) &&
			    (strlen(Parameter2) == 0))
				QuitRequest = 1;
			else
				goto syntax_error;
			break;
		case 'R':
			if (strlen(Parameter2) != 0)
				goto TooMuchParameters;
			if (STREQ(Command, "RB")) {
				if (strlen(Parameter1) == 0)
					ClearBreakpoint(cpu8051.pc);
				else
					ClearBreakpoint(
						Ascii2Hex(Parameter1, 4));
			} else
				goto syntax_error;
			break;
		case 'S':
			if (strlen(Parameter2) != 0)
				goto TooMuchParameters;

			if (STREQ(Command, "SB")) {
				if (strlen(Parameter1) == 0)
					SetBreakpoint(cpu8051.pc);
				else
					SetBreakpoint(Ascii2Hex(Parameter1, 4));
			} else
				goto syntax_error;
			break;
		case 'T':
			if (strlen(Parameter2) != 0)
				printf("Wrong Number of Parameters!\n");

			if (STREQ(Command, "T"))
				console_trace(Parameter1);
			else
				goto syntax_error;
			break;
		case 'U':
			if (STREQ(Command, "U"))
				Disasm(Parameter1, Parameter2);
			else
				goto syntax_error;
			break;
		case 'Z':
			if (STREQ(Command, "Z") && (strlen(Parameter1) == 0) &&
			    (strlen(Parameter2) == 0))
				cpu8051_Reset();
			else
				goto syntax_error;
			break;
		case '\n':
			break;
		default:
			goto syntax_error;
		}
		continue;

syntax_error:
		printf("Syntax Error!\n");
		continue;
TooMuchParameters:
		printf("Wrong Number of Parameters!\n");
		continue;
	}

	if (line)
		free(line);
}

int
main(int argc, char **argv)
{
	parse_command_line_options(argc, argv);

	cpu8051_init();

	if (options.filename != NULL)
		LoadHexFile(options.filename);

	console_main();

	log_info("Terminate");

	return 0;
}
