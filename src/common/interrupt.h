/*
 * interrupt.h
 *
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#ifndef INTERRUPT_H
#define INTERRUPT_H 1

#include <stdint.h>

#define INTERRUPT_LATENCY	10

#define INTERRUPTS_COUNT	5

#define INTERRUPT_PRIORITY_NONE	-1
#define INTERRUPT_PRIORITY_LOW	0
#define INTERRUPT_PRIORITY_HIGH	1

#define INTERRUPT_VECTOR_BASE	0x03
#define INTERRUPT_VECTOR_SIZE	0x08

#define INTERRUPT_MASK(n) (1 << (n))

void
interrupt(int *index, int *priority);

void
interrupt_address(int index, unsigned int *address);

#endif /* INTERRUPT_H */
