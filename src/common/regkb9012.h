/*
 * regkb9012.h
 *
 * Copyright (C) 2016 Paul Kocialkowski <contact@paulk.fr>
 *
 * This file is released under the GPLv2
 */

#ifndef REGKB9012_H
#define REGKB9012_H 1

/* SFR */
#define _IVHA_      0x85
#define _PCON2_     0x86
#define _SCON2_     0x9A
#define _SCON3_     0x9B
#define _SCON4_     0x9C
#define _P0IF_      0xD8
#define _P1IF_      0xE8
#define _P3IF_      0xF8

#define PCON2_FLAG_EXT_ENABLE (1 << 4)

/* External */
#define _XRAM_ 0xEC00

#define _GPIO_FS10_ 0xFC02
#define _GPIO_OE10_ 0xFC12
#define _GPIO_IE10_ 0xFC62

#define _XBISEG0_ 0xFEA0
#define _XBISEG3_ 0xFEA1
#define _XBIMISC_ 0xFEB9

#define _CLKCFG_ 0xFF0D
#define _PLLCFG_ 0xFF0F
#define _CLKCFG2_ 0xFF1E
#define _PLLCFG2_ 0xFF1F

#define XRAM_SIZE 0x1000

#define GPIO_FLAG_E51TXD (1 << 6)
#define GPIO_FLAG_E51RXD (1 << 7)

#define XBISEG_FLAG_ENABLE (1 << 7)
#define XBISEG_ADDRESS_MASK 0x3FFF
#define XBISEG_MASK 0x3F
#define XBISEG_SIZE 0x4000

#endif /* REGKB9012_H */
