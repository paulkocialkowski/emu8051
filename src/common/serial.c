/*
 * serial.c -- Serial functions.
 *
 * Copyright (C) 2015 Paul Kocialkowski <contact@paulk.fr>
 *
 * This file is released under the GPLv2
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>

#include "common.h"
#include "reg8051.h"
#include "serial.h"

#if HAVE_INTERACTIVE
#include <limits.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <pty.h>
#endif

static FILE *serial_fp = NULL;

#if HAVE_INTERACTIVE
static int serial_fd = -1;
#endif

void
serial_open(const char *serial)
{
	if (serial == NULL) {
		log_err("No file specified for serial output");
		return;
	}

	serial_fp = fopen(serial, "w");
	if (serial_fp == NULL) {
		log_err("Error opening serial output <%s>: %s", serial,
			strerror(errno));
		return;
	}

	setbuf(serial_fp, NULL);

	log_info("Outputting serial to %s", serial);
}

void
serial_close(void)
{
	if (serial_fp == NULL)
		return;

	fclose(serial_fp);

	serial_fp = NULL;
}

#if HAVE_INTERACTIVE
void
serial_interactive_open(void)
{
	struct termios termios;
	char pty_path[PATH_MAX];
	int fd = -1;
	int rc;

	cfmakeraw(&termios);
	cfsetispeed(&termios, B115200);
	cfsetospeed(&termios, B115200);

	termios.c_cflag &= ~(PARENB | CSTOPB | CSIZE | CRTSCTS);
	termios.c_cflag |= (CS8 | CLOCAL | CREAD);
	termios.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	termios.c_iflag &= ~(IXON | IXOFF | IXANY | ICRNL | IGNCR | INLCR);
	termios.c_oflag &= ~OPOST;

	rc = openpty(&serial_fd, &fd, pty_path, &termios, NULL);
	if (rc < 0) {
		log_err("Error creating interactive serial pty: %s",
			strerror(errno));
		return;
	}

	fcntl(serial_fd, F_SETFL, O_NONBLOCK);
	tcflush(serial_fd, TCIOFLUSH);

	log_info("Interactive serial served at %s", pty_path);
}

void
serial_interactive_close(void)
{
	if (serial_fd < 0)
		return;

	close(serial_fd);
	serial_fd = -1;
}
#endif

void
serial_memory_read(void)
{
#if HAVE_INTERACTIVE
	uint8_t value;
	int rc;

	if (serial_fd < 0 || mem_read_bit(SCON_BIT_RI, true))
		return;

	rc = read(serial_fd, &value, sizeof(value));
	if (rc <= 0)
		return;

	mem_write8(SFR_MEM_ID, _SBUF_, value, true);
	mem_write_bit(SCON_BIT_RI, 1, true);
#endif
}

void
serial_memory_write(enum mem_id_t id, unsigned int address, uint8_t value)
{
	if (id != SFR_MEM_ID || address != _SBUF_)
		return;

	if (serial_fp != NULL) {
		fputc(value, serial_fp);
		mem_write_bit(SCON_BIT_TI, 1, true);
	}

#if HAVE_INTERACTIVE
	if (serial_fd >= 0) {
		write(serial_fd, &value, sizeof(value));
		mem_write_bit(SCON_BIT_TI, 1, true);
	}
#endif
}
