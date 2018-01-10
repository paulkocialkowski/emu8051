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

#define IVHA_MASK (0x07)

#define PCON2_FLAG_EXT_ENABLE (1 << 4)

/* External */
#define XRAM_BASE 0xEC00

#define GPIO_FS_BASE	0xFC00
#define GPIO_OE_BASE	0xFC10
#define GPIO_D_BASE	0xFC20
#define GPIO_IN_BASE	0xFC30
#define GPIO_PU_BASE	0xFC40
#define GPIO_OD_BASE	0xFC50
#define GPIO_IE_BASE	0xFC60

#define GPIO_MISC	0xFC70
#define GPIO_MISC2	0xFC71
#define GPIO_LED	0xFC74
#define GPIO_FDA	0xFC75

#define GPIO_FS10 0xFC02
#define GPIO_OE10 0xFC12
#define GPIO_IE10 0xFC62

#define XBISEG0 0xFEA0
#define XBISEG3 0xFEA1
#define XBIMISC 0xFEB9

#define CLKCFG 0xFF0D
#define PLLCFG 0xFF0F
#define CLKCFG2 0xFF1E
#define PLLCFG2 0xFF1F

#define XRAM_SIZE 0x1000

#define GPIO_BASE_MASK						0xc0
#define GPIO_BASE_A0						0x80
#define GPIO_BASE_D0						0xc0

#define GPIO_OFFSET_SHIFT					3
#define GPIO_OFFSET_SPAN					0x0f
#define GPIO_OFFSET_MASK_A0					(1 << 0)
#define GPIO_OFFSET_A0						0x0c
#define GPIO_OFFSET_D0						0x0f

#define GPIO_BIT_MASK						0x07

#define GPIO_E51TXD						0x16
#define GPIO_E51RXD						0x17

#define GPIO_FLAG_E51TXD (1 << 6)
#define GPIO_FLAG_E51RXD (1 << 7)

#define XBISEG_FLAG_ENABLE (1 << 7)
#define XBISEG_ADDRESS_MASK 0x3FFF
#define XBISEG_MASK 0x3F
#define XBISEG_SIZE 0x4000

#endif /* REGKB9012_H */
