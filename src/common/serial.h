/*
 * serial.h
 *
 * Copyright (C) 2015 Paul Kocialkowski <contact@paulk.fr>
 *
 * This file is released under the GPLv2
 */

#ifndef SERIAL_H
#define SERIAL_H 1

#include <stdint.h>

#include "memory.h"

void
serial_open(const char *serial);

void
serial_close(void);

#if HAVE_INTERACTIVE
void
serial_interactive_open(void);

void
serial_interactive_close(void);
#endif

void
serial_memory_read(void);

void
serial_memory_write(enum mem_id_t id, unsigned int address, uint8_t value);

#endif /* IOTRACE_H */
