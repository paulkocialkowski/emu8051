/*
 * device.h
 *
 * Copyright (C) 2015 Paul Kocialkowski <contact@paulk.fr>
 *
 * This file is released under the GPLv2
 */

#ifndef DEVICE_H
#define DEVICE_H 1

bool
device_enabled(void);

void
device_open(const char *arg);

void
device_close(void);

void
device_interrupt(int *index, int *priority);

void
device_memory_read(enum mem_id_t id, unsigned int address, uint8_t *value);

void
device_memory_write(enum mem_id_t id, unsigned int address, uint8_t value);

#endif /* DEVICE_H */
