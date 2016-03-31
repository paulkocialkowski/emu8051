/*
 * kb9012.h
 *
 * Copyright (C) 2015 Paul Kocialkowski <contact@paulk.fr>
 *
 * This file is released under the GPLv2
 */

#ifndef KB9012_H
#define KB9012_H 1

void
kb9012_interrupt_address(int index, unsigned int *address);

bool
kb9012_memory_allowed(enum mem_id_t id, unsigned int address, bool write);

uint8_t
kb9012_memory_write_filter(enum mem_id_t id, unsigned int address, uint8_t value);

void
kb9012_memory_map(enum mem_id_t *id, unsigned long *address);

void
kb9012_memory_read(enum mem_id_t id, unsigned int address, uint8_t *value);

void
kb9012_memory_write(enum mem_id_t id, unsigned int address, uint8_t value);

#endif /* KB9012_H */
