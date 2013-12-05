/*
 * cpu8051.c
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

/* Define only here, for not having extern scope on local variables. */
#define CPU8051_M 1

#include <stdio.h>
#include <stdint.h>

#include "reg8051.h"
#include "cpu8051.h"
#include "memory.h"
#include "psw.h"
#include "timers.h"
#include "disasm.h"
#include "options.h"
#include "instructions_8051.h"

extern struct options_t options;

/* Check if the address is a breakpoint */
int
IsBreakpoint(unsigned int address)
{
	int k;

	for (k = 0; k < cpu8051.bp_count; k++) {
		if (cpu8051.bp[k] == address)
			return 1;
	}

	/* The address was not found in the list of breakpoints */
	return 0;
}

/* Check if the address is a stop point */
int
IsStoppoint(unsigned int address)
{
	if ((options.stop_address != 0) && (options.stop_address == address))
		return 1;
	else
		return 0;
}

/* Show Breakpoints list */
void
ShowBreakpoints(void)
{
	int k;

	for (k = 0; k < cpu8051.bp_count; k++)
		printf("Breakpoint at address = %.4X\n", cpu8051.bp[k]);
}

/* Set Breakpoint at address at the end of the breakpoint list */
void
SetBreakpoint(unsigned int address)
{
	if (IsBreakpoint(address))
		return; /* Already a breakpoint */

	if (cpu8051.bp_count < MAXBP)
		cpu8051.bp[cpu8051.bp_count++] = address;
}

/* Clear Breakpoint at Address from list */
void
ClearBreakpoint(unsigned int address)
{
	int k;

	for (k = 0; k < cpu8051.bp_count; k++) {
		if (cpu8051.bp[k] == address) {
			/* Fill removed breakpoint slot with last entry */
			cpu8051.bp[k] = cpu8051.bp[cpu8051.bp_count - 1];
			cpu8051.bp_count--;
		}
	}
}

/* Toggle the breakpoint at Address. */
void
ToggleBreakpoint(unsigned int address)
{
	if (IsBreakpoint(address))
		ClearBreakpoint(address);
	else
		SetBreakpoint(address);
}

void
cpu8051_init(void)
{
	memory_init();

	cpu8051.pc = 0;
	cpu8051.clock = 0;
	cpu8051.active_priority = -1;
	cpu8051.bp_count = 0;
}

/* Reset the registers and CPU state */
void
cpu8051_Reset(void)
{
	cpu8051.pc = 0;
	cpu8051.clock = 0;
	cpu8051.active_priority = -1;

	/* Clear IRAM and SFR. */
	memory_clear(INT_MEM_ID);

	memory_sfr_write8(_P0_, 0xFF);
	memory_sfr_write8(_P1_, 0xFF);
	memory_sfr_write8(_P2_, 0xFF);
	memory_sfr_write8(_P3_, 0xFF);

	/* The default value of SP (after system reset) is 07 */
	memory_sfr_write8(_SP_, 0x07);
}

static void
cpu8051_convert_bit_address(uint8_t bit_address, uint8_t *byte_address,
			    uint8_t *bit_number)
{
	if (bit_address > 0x7F) {
		/* SFR 80-FF */
		*byte_address = bit_address & 0xF8;
		*bit_number = bit_address & 0x07;
	} else {
		/* 20-2F */
		*byte_address = (bit_address >> 3) + 0x20;
		*bit_number = bit_address & 0x07;
	}
}

/* Write with a direct addressing mode at Address the new Value */
void
cpu8051_WriteD(unsigned int Address, unsigned char Value)
{
	memory_write8(INT_MEM_ID, Address, Value);
}

/* Write with an indirect addressing mode at Address the new Value */
void
cpu8051_WriteI(unsigned int Address, unsigned char Value)
{
	if (Address > 0x7F) {
		memory_write8(EXT_MEM_ID, Address, Value);
		return;
	}

	memory_write8(INT_MEM_ID, Address, Value);
}

/* Write with a bit addressing mode at BitAddress the new Value */
void
cpu8051_WriteB(uint8_t bit_address, uint8_t value)
{
	uint8_t byte_address;
	uint8_t bit_number;
	unsigned char ByteValue, ByteMask;

	cpu8051_convert_bit_address(bit_address, &byte_address, &bit_number);

	ByteMask = ((1 << bit_number) ^ 0xFF);
	ByteValue = cpu8051_ReadD(byte_address) & ByteMask;
	ByteValue += value << bit_number;
	cpu8051_WriteD(byte_address, ByteValue);
}

/* Read with a direct addressing mode at Address */
unsigned char
cpu8051_ReadD(unsigned int Address)
{
	if (Address > 0xFF)
		return memory_read8(EXT_MEM_ID, Address);
	else
		return memory_read8(INT_MEM_ID, Address);
}

/* Read with a indirect addressing mode at Address */
unsigned char
cpu8051_ReadI(unsigned int Address)
{
	if (Address > 0x7F)
		return memory_read8(EXT_MEM_ID, Address);
	else
		return memory_read8(INT_MEM_ID, Address);
}

/* Read with a bit addressing mode at BitAddress */
unsigned char
cpu8051_ReadB(uint8_t bit_address)
{
	uint8_t byte_address;
	uint8_t bit_number;
	unsigned char BitValue;

	cpu8051_convert_bit_address(bit_address, &byte_address, &bit_number);

	BitValue = (cpu8051_ReadD(byte_address) >> bit_number);
	BitValue &= 1;
	return BitValue;
}

static void
cpu8051_process_interrupt(int pc, int pri)
{
	stack_push16(cpu8051.pc);
	cpu8051.pc = pc;
	cpu8051.active_priority = pri;
}


/* Check interrupts state and process them as needed */
static void
cpu8051_CheckInterrupts(void)
{
	int i;

	if ((cpu8051_ReadD(_IE_) & 0x80) == 0)
		return;

	for (i = 1; i >= 0; i--) {
		if (cpu8051.active_priority < i) {
			/* Interrupt timer 0 */
			if ((cpu8051_ReadD(_IE_) & 0x02) &&
			    ((cpu8051_ReadD(_IP_ & 0x02) ? i : !i) &&
			     (cpu8051_ReadD(_TCON_) & 0x20))) {
				cpu8051_WriteD(_TCON_,
					       cpu8051_ReadD(_TCON_) & 0xDF);
				cpu8051_process_interrupt(0x0B, i);
				return;
			}
			/* Interrupt timer 1 */
			if ((cpu8051_ReadD(_IE_) & 0x08) &&
			    ((cpu8051_ReadD(_IP_) & 0x08) ? i : !i) &&
			    (cpu8051_ReadD(_TCON_) & 0x80)) {
				cpu8051_WriteD(_TCON_,
					       cpu8051_ReadD(_TCON_) & 0x7F);
				cpu8051_process_interrupt(0x1B, i);
				return;
			}
			/* Serial Interrupts */
			if ((cpu8051_ReadD(_IE_) & 0x10) &&
			    ((cpu8051_ReadD(_IP_) & 0x10) ? i : !i) &&
			    (cpu8051_ReadD(_SCON_) & 0x03)) {
				cpu8051_process_interrupt(0x23, i);
				return;
			}
			/* Interrupt timer 2 */
			if ((cpu8051_ReadD(_IE_) & 0x20) &&
			    ((cpu8051_ReadD(_IP_) & 0x20) ? i : !i) &&
			    (cpu8051_ReadD(_T2CON_) & 0x80)) {
				cpu8051_process_interrupt(0x2B, i);
				return;
			}
		}
	}
}

/* Execute at address cpu8051.pc from PGMMem */
void
cpu8051_Exec(void)
{
	int i;
	unsigned char opcode;
	int insttiming;

	opcode = memory_read8(PGM_MEM_ID, cpu8051.pc);
	cpu8051.pc++;
	insttiming = (*opcode_table[opcode])(); /* Function callback. */

	/*
	 * Parity bit (p): is automatically set or cleared in each machine
	 * cycle to establish even parity in the accumulator.
	 */
	psw_compute_parity_bit();

	for (i = 0; i < insttiming; i++) {
		cpu8051_CheckInterrupts();
		timers_check();
		cpu8051.clock++;
	}
}

/*
 * Addressing modes defined in the order as they appear in disasm.h
 * from table argstext[]
 */
#define ADDR11 0
#define ADDR16 1
#define DIRECT 3
#define BITADDR 14
#define RELADDR 15
#define DATAIMM 16
#define DATA16 22
#define CBITADDR 23

/*
 * SFR Memory map [80h - FFh]
 * ---------------------------------------------------------------
 * F8 |      |      |      |      |      |      |      |      | FF
 * F0 |   B  |      |      |      |      |      |      |      | F7
 * E8 |      |      |      |      |      |      |      |      | EF
 * E0 |  ACC |      |      |      |      |      |      |      | E7
 * D8 |      |      |      |      |      |      |      |      | DF
 * D0 |  PSW |      |      |      |      |      |      |      | D7
 * C8 | T2CON|      |RCAP2L|RCAP2H|  TL2 |  TH2 |      |      | CF
 * C0 |      |      |      |      |      |      |      |      | C7
 * B8 |  IP  |      |      |      |      |      |      |      | BF
 * B0 |  P3  |      |      |      |      |      |      |      | B7
 * A8 |  IE  |      |      |      |      |      |      |      | AF
 * A0 |  P2  |      |      |      |      |      |      |      | A7
 * 98 | SCON | SBUF |      |      |      |      |      |      | 9F
 * 90 |  P1  |      |      |      |      |      |      |      | 97
 * 88 | TCON | TMOD |  TL0 |  TL1 |  TH0 |  TH1 |      |      | 8F
 * 80 |  P0  |  SP  |  DPL |  DPH |      |      |      | PCON | 87
 * ---------------------------------------------------------------
 */

/* Return as Text the name of the SFR register at Address if any */
static int
cpu8051_SFRMemInfo(unsigned int Address, char *Text)
{
	switch (Address) {
	case 0x80: return sprintf(Text, "P0");
	case 0x81: return sprintf(Text, "SP");
	case 0x82: return sprintf(Text, "DPL");
	case 0x83: return sprintf(Text, "DPH");
	case 0x87: return sprintf(Text, "PCON");
	case 0x88: return sprintf(Text, "TCON");
	case 0x89: return sprintf(Text, "TMOD");
	case 0x8A: return sprintf(Text, "TL0");
	case 0x8B: return sprintf(Text, "TL1");
	case 0x8C: return sprintf(Text, "TH0");
	case 0x8D: return sprintf(Text, "TH1");
	case 0x90: return sprintf(Text, "P1");
	case 0x98: return sprintf(Text, "SCON");
	case 0x99: return sprintf(Text, "SBUF");
	case 0xA0: return sprintf(Text, "P2");
	case 0xA8: return sprintf(Text, "IE");
	case 0xB0: return sprintf(Text, "P3");
	case 0xB8: return sprintf(Text, "IP");
	case 0xC8: return sprintf(Text, "T2CON");
	case 0xCA: return sprintf(Text, "RCAP2L");
	case 0xCB: return sprintf(Text, "RCAP2H");
	case 0xCC: return sprintf(Text, "TL2");
	case 0xCD: return sprintf(Text, "TH2");
	case 0xD0: return sprintf(Text, "PSW");
	case 0xE0: return sprintf(Text, "ACC");
	case 0xF0: return sprintf(Text, "B");
	default: return sprintf(Text, "%.2XH", Address);
	}
}

/* Return as Text the decoded BitAddress */
static void
cpu8051_IntMemBitInfo(uint8_t bit_address, char *text)
{
	uint8_t byte_address;
	uint8_t bit_number;
	int len;

	cpu8051_convert_bit_address(bit_address, &byte_address, &bit_number);

	len = cpu8051_SFRMemInfo(byte_address, text);
	sprintf(&text[len], ".%X", bit_address);
}

/* Get instruction size from opcode */
int
cpu8051_get_instruction_size(unsigned char opcode)
{
	return InstSizesTbl[opcode];
}

/* Display instruction mnemonic. */
void
cpu8051_disasm_mnemonic(unsigned char OpCode, char *buf)
{
	sprintf(buf, "%s", InstTextTbl[InstTypesTbl[OpCode]]);
}

/* Disasm instruction arguments starting at address into a text string */
void
cpu8051_disasm_args(unsigned int address, char *buf)
{
	int len = 0;
	char TextTmp[20];
	unsigned char OpCode;
	int ArgTblOfs;
	int i;

	OpCode = memory_read8(PGM_MEM_ID, address);
	ArgTblOfs = OpCode << 2;
	address++;

	/*
	 * MOV direct, direct (OpCode 85h) is peculiar, the operands
	 * are inverted
	 */
	if (OpCode == 0x85) {
		cpu8051_SFRMemInfo(memory_read8(PGM_MEM_ID, address + 1),
				   TextTmp);
		len += sprintf(&buf[len], "%s,", TextTmp);
		cpu8051_SFRMemInfo(memory_read8(PGM_MEM_ID, address),
				   TextTmp);
		len += sprintf(&buf[len], "%s", TextTmp);
		address += 2;
		return;
	}

	for (i = 1; i <= InstArgTbl[ArgTblOfs]; i++) {
		switch (InstArgTbl[ArgTblOfs + i]) {
		case ADDR11: {
			len += sprintf(&buf[len],
				       "%.4XH", ((OpCode << 3) & 0xF00) +
				       (memory_read8(PGM_MEM_ID, address)));
			address++;
			break;
		}
		case ADDR16: {
			len += sprintf(
				&buf[len], "%.4XH",
				((memory_read8(PGM_MEM_ID, address) << 8) +
				 memory_read8(PGM_MEM_ID, address + 1)));
			address += 2;
			break;
		}
		case DIRECT: {
			cpu8051_SFRMemInfo(memory_read8(PGM_MEM_ID, address),
					   TextTmp);
			len += sprintf(&buf[len], "%s", TextTmp);
			address++;
			break;
		}
		case BITADDR: {
			cpu8051_IntMemBitInfo(
				(memory_read8(PGM_MEM_ID, address) & 0xF8),
				TextTmp);
			len += sprintf(&buf[len], "%s.%X" , TextTmp,
				       (memory_read8(PGM_MEM_ID, address) & 7));
			address++;
			break;
		}
		case RELADDR: {
			address++;
			len += sprintf(&buf[len], "%.4XH", (address & 0xFF00) +
				       (((address & 0xFF) +
					 memory_read8(PGM_MEM_ID,
						      address - 1)) & 0xFF));
			break;
		}
		case DATAIMM: {
			len += sprintf(&buf[len], "#%.2XH",
				       memory_read8(PGM_MEM_ID, address));
			address++;
			break;
		}
		case DATA16: {
			len += sprintf(&buf[len], "#%.4XH",
				       ((memory_read8(PGM_MEM_ID,
						      address) << 8) +
					memory_read8(PGM_MEM_ID, address+1)));
			address += 2;
			break;
		}
		case CBITADDR: {
			cpu8051_IntMemBitInfo((memory_read8(PGM_MEM_ID,
							    address) & 0xF8),
					      TextTmp);
			len += sprintf(&buf[len], "/%s.%X", TextTmp,
				       (memory_read8(PGM_MEM_ID, address) & 7));
			address++;
			break;
		}
		default: {
			len += sprintf(&buf[len], "%s",
				       ArgsTextTbl[InstArgTbl[ArgTblOfs + i]]);
		}
		}
		if (i < InstArgTbl[ArgTblOfs])
			len += sprintf(&buf[len], ",");
	}
}

/* Disasm one instruction at Address into a Text string */
int
cpu8051_Disasm(unsigned int Address, char *Text)
{
	int len = 0;
	char TextTmp[20];
	unsigned char OpCode;
	int ArgTblOfs;
	int InstSize;
	int i;

	/* Display address. */
	len += sprintf(Text, " %.4X ", Address);

	OpCode = memory_read8(PGM_MEM_ID, Address);
	InstSize = InstSizesTbl[OpCode];

	/* Display hex bytes. */
	for (i = 0; i < InstSize; i++)
		len += sprintf(&Text[len], " %.2X",
				      memory_read8(PGM_MEM_ID, Address + i));

	Address++;

	/* Padd remaining area with spaces. */
	for (; len < 17;)
		len += sprintf(&Text[len], " ");

	/* Display instruction mnemonic. */
	len += sprintf(&Text[len], "%s ",
			      InstTextTbl[InstTypesTbl[OpCode]]);
	ArgTblOfs = OpCode << 2;

	/* Padd remaining area with spaces. */
	for (; len < 25;)
		len += sprintf(&Text[len], " ");

	/* Display instruction arguments. */

	/*
	 * MOV direct, direct (OpCode 85h) is peculiar, the operands
	 * are inverted
	 */
	if (OpCode == 0x85) {
		cpu8051_SFRMemInfo(memory_read8(PGM_MEM_ID, Address + 1),
				   TextTmp);
		len += sprintf(&Text[len], "%s,", TextTmp);
		cpu8051_SFRMemInfo(memory_read8(PGM_MEM_ID, Address),
				   TextTmp);
		len += sprintf(&Text[len], "%s", TextTmp);
		Address += 2;
		return InstSize;
	}

	for (i = 1; i <= InstArgTbl[ArgTblOfs]; i++) {
		switch (InstArgTbl[ArgTblOfs + i]) {
		case ADDR11: {
			len += sprintf(&Text[len],
				       "%.4XH", ((OpCode << 3) & 0xF00) +
				       (memory_read8(PGM_MEM_ID, Address)));
			Address++;
			break;
		}
		case ADDR16: {
			len += sprintf(
				&Text[len], "%.4XH",
				((memory_read8(PGM_MEM_ID, Address) << 8) +
				 memory_read8(PGM_MEM_ID, Address + 1)));
			Address += 2;
			break;
		}
		case DIRECT: {
			cpu8051_SFRMemInfo(memory_read8(PGM_MEM_ID, Address),
					   TextTmp);
			len += sprintf(&Text[len], "%s", TextTmp);
			Address++;
			break;
		}
		case BITADDR: {
			cpu8051_IntMemBitInfo(
				(memory_read8(PGM_MEM_ID, Address) & 0xF8),
				TextTmp);
			len += sprintf(&Text[len], "%s.%X" , TextTmp,
				       (memory_read8(PGM_MEM_ID, Address) & 7));
			Address++;
			break;
		}
		case RELADDR: {
			Address++;
			len += sprintf(&Text[len], "%.4XH", (Address & 0xFF00) +
				       (((Address & 0xFF) +
					 memory_read8(PGM_MEM_ID,
						      Address - 1)) & 0xFF));
			break;
		}
		case DATAIMM: {
			len += sprintf(&Text[len], "#%.2XH",
				       memory_read8(PGM_MEM_ID, Address));
			Address++;
			break;
		}
		case DATA16: {
			len += sprintf(&Text[len], "#%.4XH",
				       ((memory_read8(PGM_MEM_ID,
						      Address) << 8) +
					memory_read8(PGM_MEM_ID, Address+1)));
			Address += 2;
			break;
		}
		case CBITADDR: {
			cpu8051_IntMemBitInfo((memory_read8(PGM_MEM_ID,
							    Address) & 0xF8),
					      TextTmp);
			len += sprintf(&Text[len], "/%s.%X", TextTmp,
				       (memory_read8(PGM_MEM_ID, Address) & 7));
			Address++;
			break;
		}
		default: {
			len += sprintf(&Text[len], "%s",
				       ArgsTextTbl[InstArgTbl[ArgTblOfs + i]]);
		}
		}
		if (i < InstArgTbl[ArgTblOfs])
			len += sprintf(&Text[len], ",");
	}

	return InstSize;
}
