/*
 * hardware.h
 *
 * Copyright (C) 2016 Paul Kocialkowski <contact@paulk.fr>
 *
 * This file is released under the GPLv2
 */

#ifndef HARDWARE_H
#define HARDWARE_H 1

#include <stdint.h>

#include "memory.h"

void
hardware_tick(void);

void
hardware_interrupt(int *index, int *priority);

void
hardware_interrupt_address(int index, unsigned int *address);

void
hardware_memory_map(enum mem_id_t *id, unsigned long *address);

void
hardware_memory_read(enum mem_id_t id, unsigned int address, uint8_t *value);

void
hardware_memory_write(enum mem_id_t id, unsigned int address, uint8_t value);

#endif /* HARDWARE_H */
