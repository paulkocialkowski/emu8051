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
#include "reg8051.h"
#include "device.h"
#include "serial.h"
#include "hardware.h"

void
hardware_memory_map(enum mem_id_t *id, unsigned long *address)
{
}

void
hardware_memory_read(enum mem_id_t id, unsigned int address, uint8_t *value)
{
#if HAVE_DEVICE
	device_memory_read(id, address, value);
#endif
}

void
hardware_memory_write(enum mem_id_t id, unsigned int address, uint8_t value)
{
#if HAVE_DEVICE
	device_memory_write(id, address, value);
#endif

	serial_memory_write(id, address, value);
}


