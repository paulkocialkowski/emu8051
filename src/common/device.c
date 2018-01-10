/*
 * device.c -- Device functions.
 *
 * Copyright (C) 2015 Paul Kocialkowski <contact@paulk.fr>
 *
 * This file is released under the GPLv2
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdbool.h>
#include <emu8051-device.h>

#include "common.h"
#include "interrupt.h"
#include "memory.h"
#include "kb9012.h"
#include "reg8051.h"

static struct emu8051_device device;
static int device_opened = 0;

static int
device_memory(enum mem_id_t id)
{
	switch (id) {
		case PGM_MEM_ID:
			return EMU8051_DEVICE_MEMORY_PROGRAM;
		case INT_MEM_ID:
			return EMU8051_DEVICE_MEMORY_INTERNAL;
		case SFR_MEM_ID:
			return EMU8051_DEVICE_MEMORY_SFR;
		case EXT_MEM_ID:
			return EMU8051_DEVICE_MEMORY_EXTERNAL;
		default:
			return -1;
	}
}

static bool
device_memory_allowed(enum mem_id_t id, unsigned int address, bool write)
{
#if HAVE_KB9012
	bool allowed;

	allowed = kb9012_memory_allowed(id, address, write);
	if (!allowed)
		return false;
#endif

	switch (id) {
		/* Program and internal memory are used by the target program */
		case PGM_MEM_ID:
		case INT_MEM_ID:
			return false;
		case SFR_MEM_ID:
			switch (address) {
				/* Those are emulated. */
				case _SP_:
				case _DPL_:
				case _DPH_:
				case _PCON_:
				case _TMOD_:
				case _TL0_:
				case _TL1_:
				case _TH0_:
				case _TH1_:
				case _SCON_:
				case _SBUF_:
				case _P2_:
				case _PSW_:
				case _ACC_:
				case _B_:
					return false;
				/*
				 * Some interrupts can be enabled (write) but
				 * the value will be filtered regarding what
				 * flags need to be kept and what is emulated.
				 * Since the target won't modify IE or IP,
				 * reading those from cache is sufficient.
				 */
				case _IE_:
				case _IP_:
					return write;
				default:
					return true;
			}
		default:
			return true;
	}
}

static uint8_t
device_memory_write_filter(enum mem_id_t id, unsigned int address, uint8_t value)
{
	uint8_t flags_always = 0;
	uint8_t flags_never = 0;

#if HAVE_KB9012
	value = kb9012_memory_write_filter(id, address, value);
#endif

	switch (id) {
		case SFR_MEM_ID:
			switch (address) {
				case _TCON_:
					/* Those are emulated. */

					flags_always = TCON_FLAG_TF1 |
						       TCON_FLAG_TF0;

					flags_never = TCON_FLAG_TR1 |
						      TCON_FLAG_TR0;
					break;
				case _IE_:
					/* Those are required by the target. */
					flags_always = IE_FLAG_EA | IE_FLAG_ES;
					/* Those are emulated. */
					flags_never = IE_FLAG_ET0 |IE_FLAG_ET1;
					break;
				case _IP_:
					/* Those are emulated. */
					flags_never = INTERRUPT_MASK(4) |
						      INTERRUPT_MASK(3) |
						      INTERRUPT_MASK(1);
					break;
			}
			break;
		default:
			break;
	}

	return (value | flags_always) & ~flags_never;
}

static uint8_t
device_memory_read_preserve(enum mem_id_t id, unsigned int address,
			    uint8_t value, uint8_t value_preserve)
{
	uint8_t flags_preserve = 0;

	switch (id) {
		case SFR_MEM_ID:
			switch (address) {
				case _TCON_:
					/* Those are emulated. */
					flags_preserve = TCON_FLAG_TF1 |
							 TCON_FLAG_TR1 |
							 TCON_FLAG_TF0 |
							 TCON_FLAG_TR0;
					break;
			}
			break;
		default:
			break;
	}

	return (value & ~flags_preserve) | (value_preserve & flags_preserve);
}

bool
device_enabled(void)
{
	return device_opened != 0;
}

void
device_open(const char *arg)
{
	int rc;

	rc = emu8051_device_open(&device, arg);
	if (rc < 0) {
		log_err("Error opening device with argument: %s", arg);
		device_opened = 0;
		return;
	}

	log_info("Device communication with argument: %s", arg);

	device_opened = 1;
}

void
device_close(void)
{
	if (!device_opened)
		return;

	emu8051_device_close(&device);

	device_opened = 0;
}

void
device_interrupt(int *index, int *priority)
{
	if (!device_opened)
		return;

	if (*index > 0 || *priority >= INTERRUPT_PRIORITY_LOW)
		return;

	emu8051_device_interrupt(&device, index);
	*priority = INTERRUPT_PRIORITY_LOW;

	if (*index > 0)
		printf("device interrupt %d\n", *index);
}

void
device_memory_read(enum mem_id_t id, unsigned int address, uint8_t *value)
{
	uint8_t value_preserve;
	bool allowed;
	int memory;
	int rc;

	if (!device_opened)
		return;

	allowed = device_memory_allowed(id, address, false);
	if (!allowed)
		return;

	memory = device_memory(id);
	if (memory < 0)
		return;

	value_preserve = *value;

	rc = emu8051_device_memory_read(&device, memory,
					(unsigned short) address,
					(unsigned char *) value);
	if (rc < 0) {
		log_err("Error reading from device memory");
		return;
	}

	*value = device_memory_read_preserve(id, address, *value,
					     value_preserve);
}

void
device_memory_write(enum mem_id_t id, unsigned int address, uint8_t value)
{
	bool allowed;
	int memory;
	int rc;

	if (!device_opened)
		return;

	allowed = device_memory_allowed(id, address, true);
	if (!allowed)
		return;

	memory = device_memory(id);
	if (memory < 0)
		return;

	printf("MEM %d 0x%x = 0x%x\n", id, address, value);

	value = device_memory_write_filter(id, address, value);

	rc = emu8051_device_memory_write(&device, memory,
					 (unsigned short) address,
					 (unsigned char) value);
	if (rc < 0) {
		log_err("Error writing to device memory");
		return;
	}
}
