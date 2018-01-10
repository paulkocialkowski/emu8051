/*
 * hardware.c -- Hardware emulation functions.
 *
 * Copyright (C) 2016 Paul Kocialkowski <contact@paulk.fr>
 *
 * This file is released under the GPLv2
 */

#include <stdlib.h>
#include <stdbool.h>

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include "common.h"
#include "memory.h"
#include "interrupt.h"
#include "reg8051.h"
#include "kb9012.h"
#include "timers.h"
#include "device.h"
#include "serial.h"
#include "hardware.h"

void
hardware_tick(void)
{
	timers_tick();
}

void
hardware_interrupt(int *index, int *priority)
{
	interrupt(index, priority);

#if HAVE_DEVICE
	if (device_enabled())
		device_interrupt(index, priority);
#endif
}

void
hardware_interrupt_address(int index, unsigned int *address)
{
	interrupt_address(index, address);

#if HAVE_KB9012
	kb9012_interrupt_address(index, address);
#endif
}

void
hardware_memory_map(enum mem_id_t *id, unsigned long *address)
{
#if HAVE_KB9012
	kb9012_memory_map(id, address);
#endif
}

void
hardware_memory_read(enum mem_id_t id, unsigned int address, uint8_t *value)
{
#if HAVE_KB9012
	kb9012_memory_read(id, address, value);
#endif

#if HAVE_DEVICE
	if (device_enabled())
		device_memory_read(id, address, value);
#endif
}

void
hardware_memory_write(enum mem_id_t id, unsigned int address, uint8_t value)
{
#if HAVE_KB9012
	kb9012_memory_write(id, address, value);
#endif

#if HAVE_DEVICE
	if (device_enabled())
		device_memory_write(id, address, value);
#endif

	serial_memory_write(id, address, value);
}
