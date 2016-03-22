/*
 * reg8051.h
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#ifndef REG8051_H
#define REG8051_H 1

/* SFR Registers ( $80 - $FF ) */
#define _ACC_       0xE0
#define _B_         0xF0
#define _PSW_       0xD0
#define _SP_        0x81
#define _DPTRLOW_   _DPL_
#define _DPTRHIGH_  _DPH_
#define _DPL_       0x82
#define _DPH_       0x83
#define _P0_        0x80
#define _P1_        0x90
#define _P2_        0xA0
#define _P3_        0xB0
#define _IP_        0xB8
#define _IE_        0xA8
#define _TMOD_      0x89
#define _TCON_      0x88
#define _TH0_       0x8C
#define _TL0_       0x8A
#define _TH1_       0x8D
#define _TL1_       0x8B
#define _SCON_      0x98
#define _SBUF_      0x99
#define _PCON_      0x87
#define _T2CON_     0xC8

#define _R0_        0x00
#define _R1_        0x01
#define _R2_        0x02
#define _R3_        0x03
#define _R4_        0x04
#define _R5_        0x05
#define _R6_        0x06
#define _R7_        0x07

#define _BANK0_     0x00
#define _BANK1_     0x08
#define _BANK2_     0x10
#define _BANK3_     0x18

#define TCON_FLAG_TF1 (1 << 7)
#define TCON_FLAG_TR1 (1 << 6)
#define TCON_FLAG_TF0 (1 << 5)
#define TCON_FLAG_TR0 (1 << 4)
#define TCON_FLAG_IE1 (1 << 3)
#define TCON_FLAG_IE0 (1 << 1)

#define SCON_BIT_RI 0x98
#define SCON_BIT_TI 0x99

#define SCON_FLAG_RI (1 << 0)
#define SCON_FLAG_TI (1 << 1)

#define PSW_BIT_CY 7
#define PSW_BIT_AC 6
#define PSW_BIT_OV 2
#define PSW_BIT_P  0

#define PSW_FLAG_CY (1 << PSW_BIT_CY)
#define PSW_FLAG_AC (1 << PSW_BIT_AC)
#define PSW_FLAG_OV (1 << PSW_BIT_OV)
#define PSW_FLAG_P  (1 << PSW_BIT_P)

#define IE_FLAG_EA (1 << 7)
#define IE_FLAG_ES (1 << 4)
#define IE_FLAG_ET1 (1 << 3)
#define IE_FLAG_ET0 (1 << 1)

#endif /* REG8051_H */
