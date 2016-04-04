/*
 * exectrace.h
 *
 * Copyright (C) 2016 Paul Kocialkowski <contact@paulk.fr>
 *
 * This file is released under the GPLv2
 */

#ifndef EXECTRACE_H
#define EXECTRACE_H 1

#include <stdint.h>

void
exectrace_open(const char *exectrace);

void
exectrace_close(void);

void
exectrace_instruction(unsigned int address);

#endif /* EXECTRACE_H */
