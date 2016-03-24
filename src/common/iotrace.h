/*
 * iotrace.h
 *
 * Copyright (C) 2015 Paul Kocialkowski <contact@paulk.fr>
 *
 * This file is released under the GPLv2
 */

#ifndef IOTRACE_H
#define IOTRACE_H 1

#include <stdint.h>

#include "memory.h"

void
iotrace_open(const char *iotrace);

void
iotrace_close(void);

void
iotrace_interrupt(int index, unsigned int address);

void
iotrace_memory_read(enum mem_id_t id, unsigned int address, uint8_t value);

void
iotrace_memory_write(enum mem_id_t id, unsigned int address, uint8_t value);

void
iotrace_message(const char *message);

#endif /* IOTRACE_H */
