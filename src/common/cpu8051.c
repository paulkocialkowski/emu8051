/*
 * cpu8051.c
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

/* Define only here, for not having extern scope on local variables. */
#define CPU8051_M 1

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "common.h"
#include "reg8051.h"
#include "cpu8051.h"
#include "memory.h"
#include "psw.h"
#include "timers.h"
#include "opcodes.h"
#include "options.h"
#include "instructions_8051.h"

extern struct options_t options;

/* Check if the address is a breakpoint */
int
breakpoint_is_defined(unsigned int address)
{
	int k;

	for (k = 0; k < cpu8051.bp_count; k++) {
		if (cpu8051.bp[k] == address)
			return true;
	}

	/* The address was not found in the list of breakpoints */
	return false;
}

/* Show Breakpoints list */
void
breakpoints_show(void)
{
	int k;

	if (cpu8051.bp_count) {
		printf("Breakpoints:\n");
		for (k = 0; k < cpu8051.bp_count; k++)
			printf("  $%.4X\n", cpu8051.bp[k]);
	} else {
		printf("No breakpoints defined\n");
	}
}

/* Set Breakpoint at address at the end of the breakpoint list */
void
breakpoint_set(unsigned int address)
{
	int rc;

	rc = mem_check_address(PGM_MEM_ID, address, DISPLAY_ERROR_YES);
	if (!rc)
		return; /* Error */

	/* Check if breakpoint is already defined. */
	if ((breakpoint_is_defined(address) == false) &&
	    (cpu8051.bp_count < MAXBP))
		cpu8051.bp[cpu8051.bp_count++] = address;
}

/* Clear Breakpoint at Address from list */
void
breakpoint_clr(unsigned int address)
{
	int k;
	int rc;

	rc = mem_check_address(PGM_MEM_ID, address, DISPLAY_ERROR_YES);
	if (!rc)
		return; /* Error */

	/* Check if breakpoint is defined. */
	if (breakpoint_is_defined(address) == false) {
		log_err("No breakpoint defined at address $%X", address);
		return;
	}

	for (k = 0; k < cpu8051.bp_count; k++) {
		if (cpu8051.bp[k] == address) {
			/* Fill removed breakpoint slot with last entry */
			cpu8051.bp[k] = cpu8051.bp[cpu8051.bp_count - 1];
			cpu8051.bp_count--;
		}
	}
}

/* Clear all breakpoints */
void
breakpoints_clr_all(void)
{
	cpu8051.bp_count = 0;
}

/* Toggle the breakpoint at Address. */
void
breakpoint_toggle(unsigned int address)
{
	if (breakpoint_is_defined(address))
		breakpoint_clr(address);
	else
		breakpoint_set(address);
}

/* Check if the address is a stop point */
static int
stop_point_is_defined(unsigned int address)
{
	if ((options.stop_address != 0) && (options.stop_address == address))
		return true;
	else
		return false;
}

void
cpu8051_init(void)
{
	int id;

	mem_init();

	for (id = 0; id < GP_TIMERS_COUNT; id++)
		gp_timer_reset(id);

	cpu8051.pc = 0;
	cpu8051.clock = 0;
	cpu8051.active_priority = -1;
	cpu8051.bp_count = 0;
}

/* Reset the registers and CPU state */
void
cpu8051_reset(void)
{
	cpu8051.pc = 0;
	cpu8051.clock = 0;
	cpu8051.active_priority = -1;

	/* Clear IRAM and SFR. */
	mem_clear(INT_MEM_ID);

	mem_sfr_write8(_P0_, 0xFF);
	mem_sfr_write8(_P1_, 0xFF);
	mem_sfr_write8(_P2_, 0xFF);
	mem_sfr_write8(_P3_, 0xFF);

	/* The default value of SP (after system reset) is 07 */
	mem_sfr_write8(_SP_, 0x07);
}

static int
cpu8051_interrupt_fire(int interrupt_no, int priority)
{
	if (mem_read_direct(_IP_) & INTERRUPT_MASK(interrupt_no))
		return priority;
	else
		return !priority;
}

static int
cpu8051_interrupt_enabled(int interrupt_no)
{
	return (mem_read_direct(_IE_) & INTERRUPT_MASK(interrupt_no)) ?
		1 : 0;
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
cpu8051_check_interrupts(void)
{
	int i;

	if ((mem_read_direct(_IE_) & 0x80) == 0)
		return;

	for (i = INTERRUPT_PRIORITY_HIGH; i >= INTERRUPT_PRIORITY_LOW; i--) {
		if (cpu8051.active_priority < i) {
			/* Interrupt timer 0 */
			if (cpu8051_interrupt_enabled(INTERRUPT_1) &&
			    cpu8051_interrupt_fire(INTERRUPT_1, i) &&
			    (mem_read_direct(_TCON_) & 0x20)) {
				mem_write_direct(
					_TCON_,
					mem_read_direct(_TCON_) & 0xDF);
				cpu8051_process_interrupt(0x0B, i);
				return;
			}
			/* Interrupt timer 1 */
			if (cpu8051_interrupt_enabled(INTERRUPT_3) &&
			    cpu8051_interrupt_fire(INTERRUPT_3, i) &&
			    (mem_read_direct(_TCON_) & 0x80)) {
				mem_write_direct(
					_TCON_,
					mem_read_direct(_TCON_) & 0x7F);
				cpu8051_process_interrupt(0x1B, i);
				return;
			}
			/* Serial Interrupts */
			if (cpu8051_interrupt_enabled(INTERRUPT_4) &&
			    cpu8051_interrupt_fire(INTERRUPT_4, i) &&
			    (mem_read_direct(_SCON_) & 0x03)) {
				cpu8051_process_interrupt(0x23, i);
				return;
			}
			/* Interrupt timer 2 */
			if (cpu8051_interrupt_enabled(INTERRUPT_5) &&
			    cpu8051_interrupt_fire(INTERRUPT_5, i) &&
			    (mem_read_direct(_T2CON_) & 0x80)) {
				cpu8051_process_interrupt(0x2B, i);
				return;
			}
		}
	}
}

/* Execute at address cpu8051.pc from PGMMem */
int
cpu8051_exec(void)
{
	int i;
	int rc;
	unsigned char opcode;
	int insttiming;

	/* Basic address check (may fail later if opcode has operands). */
	rc = mem_check_address(PGM_MEM_ID, cpu8051.pc, DISPLAY_ERROR_NO);
	if (!rc) {
		log_err("Trying to run past program memory limit");
		return false; /* Error */
	}

	opcode = mem_read8(PGM_MEM_ID, cpu8051.pc);
	cpu8051.pc++;
	insttiming = (*opcode_table[opcode])(); /* Function callback. */

	/*
	 * Parity bit (p): is automatically set or cleared in each machine
	 * cycle to establish even parity in the accumulator.
	 */
	psw_compute_parity_bit();

	gp_timers_increment(insttiming);

	for (i = 0; i < insttiming; i++) {
		cpu8051_check_interrupts();
		timers_check();
		cpu8051.clock++;
	}

	return true;
}

/*
 * Run specified number of instructions, or when encountering a
 * breakpoint or a stop point.
 * Set instr_count to -1 to disable running for a specific number
 * of instructions.
 *
 * Returns TRUE when a breakpoint is encountered.
 */
int
cpu8051_run(int instr_count, int (*interface_stop)(void))
{
	int rc;
	int run = true;
	int breakpoint_hit = false;

	while (run) {
		rc = cpu8051_exec();
		if (!rc) {
			run = false;
		} else {
			if (instr_count > 0)
				instr_count--;

			if (instr_count == 0) {
				run = false;
				log_info("Number of instructions reached");
			}

			if (breakpoint_is_defined(cpu8051.pc)) {
				run = false;
				breakpoint_hit = true;
				log_info("Breakpoint hit at %.4X", cpu8051.pc);
			}

			if (stop_point_is_defined(cpu8051.pc)) {
				run = false;
				log_info("Stoppoint hit at %.4X", cpu8051.pc);
			}

			if (interface_stop != NULL) {
				if (interface_stop()) {
					run = false;
					log_info("Caught break signal");
				}
			}
		}
	}

	return breakpoint_hit;
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
cpu8051_sfr_mem_info(unsigned int address, char *text)
{
	switch (address) {
	case 0x80: return sprintf(text, "P0");
	case 0x81: return sprintf(text, "SP");
	case 0x82: return sprintf(text, "DPL");
	case 0x83: return sprintf(text, "DPH");
	case 0x87: return sprintf(text, "PCON");
	case 0x88: return sprintf(text, "TCON");
	case 0x89: return sprintf(text, "TMOD");
	case 0x8A: return sprintf(text, "TL0");
	case 0x8B: return sprintf(text, "TL1");
	case 0x8C: return sprintf(text, "TH0");
	case 0x8D: return sprintf(text, "TH1");
	case 0x90: return sprintf(text, "P1");
	case 0x98: return sprintf(text, "SCON");
	case 0x99: return sprintf(text, "SBUF");
	case 0xA0: return sprintf(text, "P2");
	case 0xA8: return sprintf(text, "IE");
	case 0xB0: return sprintf(text, "P3");
	case 0xB8: return sprintf(text, "IP");
	case 0xC8: return sprintf(text, "T2CON");
	case 0xCA: return sprintf(text, "RCAP2L");
	case 0xCB: return sprintf(text, "RCAP2H");
	case 0xCC: return sprintf(text, "TL2");
	case 0xCD: return sprintf(text, "TH2");
	case 0xD0: return sprintf(text, "PSW");
	case 0xE0: return sprintf(text, "ACC");
	case 0xF0: return sprintf(text, "B");
	default: return sprintf(text, "%.2XH", address);
	}
}

/* Return as text the decoded bit address */
static void
cpu8051_int_mem_bit_info(uint8_t bit_address, char *text)
{
	uint8_t byte_address;
	uint8_t bit_number;
	int len;

	mem_convert_bit_address(bit_address, &byte_address, &bit_number);

	len = cpu8051_sfr_mem_info(byte_address, text);
	sprintf(&text[len], ".%X", bit_address);
}

/* Display instruction mnemonic. */
int
cpu8051_disasm_mnemonic(unsigned char opcode, char *buf)
{
	return sprintf(buf, "%s", opcodes_get_instr_type_str(opcode));
}

/* Disasm instruction arguments starting at address into a text string */
void
cpu8051_disasm_args(unsigned int address, char *buf)
{
	int len = 0;
	char str[20];
	unsigned char opcode;
	int args_table_offset;
	int i;

	buf[0] = '\0';

	opcode = mem_read8(PGM_MEM_ID, address);
	args_table_offset = opcode << 2;
	address++;

	/*
	 * MOV direct, direct (opcode 85h) is peculiar, the operands
	 * are inverted
	 */
	if (opcode == 0x85) {
		cpu8051_sfr_mem_info(mem_read8(PGM_MEM_ID, address + 1),
				     str);
		len += sprintf(&buf[len], "%s,", str);
		cpu8051_sfr_mem_info(mem_read8(PGM_MEM_ID, address),
				     str);
		len += sprintf(&buf[len], "%s", str);
		address += 2;
		return;
	}

	for (i = 1; i <= opcodes_get_instr_arg_type_id(args_table_offset);
	     i++) {
		switch (opcodes_get_instr_arg_type_id(args_table_offset + i)) {
		case ADDR11: {
			len += sprintf(&buf[len],
				       "%.4XH", ((opcode << 3) & 0xF00) +
				       (mem_read8(PGM_MEM_ID, address)));
			address++;
			break;
		}
		case ADDR16: {
			len += sprintf(
				&buf[len], "%.4XH",
				((mem_read8(PGM_MEM_ID, address) << 8) +
				 mem_read8(PGM_MEM_ID, address + 1)));
			address += 2;
			break;
		}
		case DIRECT: {
			cpu8051_sfr_mem_info(mem_read8(PGM_MEM_ID, address),
					     str);
			len += sprintf(&buf[len], "%s", str);
			address++;
			break;
		}
		case BITADDR: {
			cpu8051_int_mem_bit_info(
				(mem_read8(PGM_MEM_ID, address) & 0xF8),
				str);
			len += sprintf(&buf[len], "%s.%X" , str,
				       (mem_read8(PGM_MEM_ID, address) & 7));
			address++;
			break;
		}
		case RELADDR: {
			address++;
			len += sprintf(&buf[len], "%.4XH", (address & 0xFF00) +
				       (((address & 0xFF) +
					 mem_read8(PGM_MEM_ID,
						   address - 1)) & 0xFF));
			break;
		}
		case DATAIMM: {
			len += sprintf(&buf[len], "#%.2XH",
				       mem_read8(PGM_MEM_ID, address));
			address++;
			break;
		}
		case DATA16: {
			len += sprintf(&buf[len], "#%.4XH",
				       ((mem_read8(PGM_MEM_ID,
						   address) << 8) +
					mem_read8(PGM_MEM_ID, address+1)));
			address += 2;
			break;
		}
		case CBITADDR: {
			cpu8051_int_mem_bit_info((mem_read8(PGM_MEM_ID,
							    address) & 0xF8),
						 str);
			len += sprintf(&buf[len], "/%s.%X", str,
				       (mem_read8(PGM_MEM_ID, address) & 7));
			address++;
			break;
		}
		default: {
			len += sprintf(&buf[len], "%s",
				       opcodes_get_instr_arg_type_str(
					       args_table_offset + i));
		}
		}
		if (i < opcodes_get_instr_arg_type_id(args_table_offset))
			len += sprintf(&buf[len], ",");
	}
}

/* Disasm one instruction at address into a Text string */
int
cpu8051_disasm(unsigned int address, char *text)
{
	int len = 0;
	unsigned char opcode;
	int inst_size;
	int i;

	/* Display address. */
	len += sprintf(text, " %.4X ", address);

	opcode = mem_read8(PGM_MEM_ID, address);
	inst_size = opcodes_get_instr_size(opcode);

	/* Display hex bytes. */
	for (i = 0; i < inst_size; i++)
		len += sprintf(&text[len], " %.2X",
			       mem_read8(PGM_MEM_ID, address + i));

	/* Padd remaining area with spaces. */
	for (; len < 17;)
		len += sprintf(&text[len], " ");

	/* Display instruction mnemonic. */
	len += cpu8051_disasm_mnemonic(opcode, &text[len]);

	/* Padd remaining area with spaces. */
	for (; len < 25;)
		len += sprintf(&text[len], " ");

	/* Display instruction arguments. */
	cpu8051_disasm_args(address, &text[len]);

	return inst_size;
}
