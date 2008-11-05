/*
 * disasm.hpp
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __DISASM_HPP_
#define __DISASM_HPP_
// Do not modify this file directly, it was created by Opcode2cpp.pl
// Any modification made directly on this file will be lost


// For all 256 opcodes, the value in this table gives the instruction type
// ex.: MOV, INC, CLR, CPL,...
// To know what is the instruction type, use the number as an offset in the InstTextTbl[]
static int InstTypesTbl[] = {
 0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
 5, 6, 7, 8, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
 10, 1, 11, 12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
 14, 6, 15, 16, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
 18, 1, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
 20, 6, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
 22, 1, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
 24, 6, 19, 25, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
 27, 1, 21, 28, 29, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
 26, 6, 26, 28, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
 19, 1, 26, 4, 31, 32, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
 21, 6, 33, 33, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
 35, 1, 36, 36, 37, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38,
 39, 6, 40, 40, 41, 42, 43, 43, 42, 42, 42, 42, 42, 42, 42, 42,
 44, 1, 44, 44, 36, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
 44, 6, 44, 44, 33, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26
};


// Size(in bytes) of each instruction (offset in table is instruction opcode)
static int InstSizesTbl[] = {
 1, 2, 3, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 3, 2, 3, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 3, 2, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 3, 2, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 2, 2, 2, 3, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 2, 2, 2, 3, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 2, 2, 2, 3, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 2, 2, 2, 1, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 1, 1, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 3, 2, 2, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 2, 2, 2, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
 2, 2, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 2, 2, 2, 1, 1, 3, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,
 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};


// List of instructions types referenced by InstTypesTbl[]
#define InstTextTblLength 45
static char *InstTextTbl[] = {
 "NOP",
 "AJMP",
 "LJMP",
 "RR",
 "INC",
 "JBC",
 "ACALL",
 "LCALL",
 "RRC",
 "DEC",
 "JB",
 "RET",
 "RL",
 "ADD",
 "JNB",
 "RETI",
 "RLC",
 "ADDC",
 "JC",
 "ORL",
 "JNC",
 "ANL",
 "JZ",
 "XRL",
 "JNZ",
 "JMP",
 "MOV",
 "SJMP",
 "MOVC",
 "DIV",
 "SUBB",
 "MUL",
 "INVALID",
 "CPL",
 "CJNE",
 "PUSH",
 "CLR",
 "SWAP",
 "XCH",
 "POP",
 "SETB",
 "DA",
 "DJNZ",
 "XCHD",
 "MOVX"
};


// Table describing all arguments types of an instruction
// The table is indexed InstArgTbl[ opcode * 4]
// InstArgTbl[opcode*4 + 1] gives the number of arguments the instruction has
// InstArgTbl[opcode*4 + i] for i=1,2 and 3 give the type of each argument
// for most instructions, the 3rd argument isn't used
// the argument type is referecing to ArgsTextTbl[]
#define InstArgTblLength 256
static int InstArgTbl[] = {
 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 2, 0, 0,
 1, 2, 0, 0, 1, 3, 0, 0, 1, 4, 0, 0, 1, 5, 0, 0,
 1, 6, 0, 0, 1, 7, 0, 0, 1, 8, 0, 0, 1, 9, 0, 0,
 1, 10, 0, 0, 1, 11, 0, 0, 1, 12, 0, 0, 1, 13, 0, 0,
 2, 14, 15, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 2, 0, 0,
 1, 2, 0, 0, 1, 3, 0, 0, 1, 4, 0, 0, 1, 5, 0, 0,
 1, 6, 0, 0, 1, 7, 0, 0, 1, 8, 0, 0, 1, 9, 0, 0,
 1, 10, 0, 0, 1, 11, 0, 0, 1, 12, 0, 0, 1, 13, 0, 0,
 2, 14, 15, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0,
 2, 2, 16, 0, 2, 2, 3, 0, 2, 2, 4, 0, 2, 2, 5, 0,
 2, 2, 6, 0, 2, 2, 7, 0, 2, 2, 8, 0, 2, 2, 9, 0,
 2, 2, 10, 0, 2, 2, 11, 0, 2, 2, 12, 0, 2, 2, 13, 0,
 2, 14, 15, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0,
 2, 2, 16, 0, 2, 2, 3, 0, 2, 2, 4, 0, 2, 2, 5, 0,
 2, 2, 6, 0, 2, 2, 7, 0, 2, 2, 8, 0, 2, 2, 9, 0,
 2, 2, 10, 0, 2, 2, 11, 0, 2, 2, 12, 0, 2, 2, 13, 0,
 1, 15, 0, 0, 1, 0, 0, 0, 2, 3, 2, 0, 2, 3, 16, 0,
 2, 2, 16, 0, 2, 2, 3, 0, 2, 2, 4, 0, 2, 2, 5, 0,
 2, 2, 6, 0, 2, 2, 7, 0, 2, 2, 8, 0, 2, 2, 9, 0,
 2, 2, 10, 0, 2, 2, 11, 0, 2, 2, 12, 0, 2, 2, 13, 0,
 1, 15, 0, 0, 1, 0, 0, 0, 2, 3, 2, 0, 2, 3, 16, 0,
 2, 2, 16, 0, 2, 2, 3, 0, 2, 2, 4, 0, 2, 2, 5, 0,
 2, 2, 6, 0, 2, 2, 7, 0, 2, 2, 8, 0, 2, 2, 9, 0,
 2, 2, 10, 0, 2, 2, 11, 0, 2, 2, 12, 0, 2, 2, 13, 0,
 1, 15, 0, 0, 1, 0, 0, 0, 2, 3, 2, 0, 2, 3, 16, 0,
 2, 2, 16, 0, 2, 2, 3, 0, 2, 2, 4, 0, 2, 2, 5, 0,
 2, 2, 6, 0, 2, 2, 7, 0, 2, 2, 8, 0, 2, 2, 9, 0,
 2, 2, 10, 0, 2, 2, 11, 0, 2, 2, 12, 0, 2, 2, 13, 0,
 1, 15, 0, 0, 1, 0, 0, 0, 2, 17, 14, 0, 1, 18, 0, 0,
 2, 2, 16, 0, 2, 3, 16, 0, 2, 4, 16, 0, 2, 5, 16, 0,
 2, 6, 16, 0, 2, 7, 16, 0, 2, 8, 16, 0, 2, 9, 16, 0,
 2, 10, 16, 0, 2, 11, 16, 0, 2, 12, 16, 0, 2, 13, 16, 0,
 1, 15, 0, 0, 1, 0, 0, 0, 2, 17, 14, 0, 2, 2, 19, 0,
 1, 20, 0, 0, 2, 3, 3, 0, 2, 3, 4, 0, 2, 3, 5, 0,
 2, 3, 6, 0, 2, 3, 7, 0, 2, 3, 8, 0, 2, 3, 9, 0,
 2, 3, 10, 0, 2, 3, 11, 0, 2, 3, 12, 0, 2, 3, 13, 0,
 2, 21, 22, 0, 1, 0, 0, 0, 2, 14, 17, 0, 2, 2, 18, 0,
 2, 2, 16, 0, 2, 2, 3, 0, 2, 2, 4, 0, 2, 2, 5, 0,
 2, 2, 6, 0, 2, 2, 7, 0, 2, 2, 8, 0, 2, 2, 9, 0,
 2, 2, 10, 0, 2, 2, 11, 0, 2, 2, 12, 0, 2, 2, 13, 0,
 2, 17, 23, 0, 1, 0, 0, 0, 2, 17, 14, 0, 1, 21, 0, 0,
 1, 20, 0, 0, 0, 0, 0, 0, 2, 4, 3, 0, 2, 5, 3, 0,
 2, 6, 3, 0, 2, 7, 3, 0, 2, 8, 3, 0, 2, 9, 3, 0,
 2, 10, 3, 0, 2, 11, 3, 0, 2, 12, 3, 0, 2, 13, 3, 0,
 2, 17, 23, 0, 1, 0, 0, 0, 1, 14, 0, 0, 1, 17, 0, 0,
 3, 2, 16, 15, 3, 2, 3, 15, 3, 4, 16, 15, 3, 5, 16, 15,
 3, 6, 16, 15, 3, 7, 16, 15, 3, 8, 16, 15, 3, 9, 16, 15,
 3, 10, 16, 15, 3, 11, 16, 15, 3, 12, 16, 15, 3, 13, 16, 15,
 1, 3, 0, 0, 1, 0, 0, 0, 1, 14, 0, 0, 1, 17, 0, 0,
 1, 2, 0, 0, 2, 2, 3, 0, 2, 2, 4, 0, 2, 2, 5, 0,
 2, 2, 6, 0, 2, 2, 7, 0, 2, 2, 8, 0, 2, 2, 9, 0,
 2, 2, 10, 0, 2, 2, 11, 0, 2, 2, 12, 0, 2, 2, 13, 0,
 1, 3, 0, 0, 1, 0, 0, 0, 1, 14, 0, 0, 1, 17, 0, 0,
 1, 2, 0, 0, 2, 3, 15, 0, 2, 2, 4, 0, 2, 2, 5, 0,
 2, 6, 15, 0, 2, 7, 15, 0, 2, 8, 15, 0, 2, 9, 15, 0,
 2, 10, 15, 0, 2, 11, 15, 0, 2, 12, 15, 0, 2, 13, 15, 0,
 2, 2, 24, 0, 1, 0, 0, 0, 2, 2, 4, 0, 2, 2, 5, 0,
 1, 2, 0, 0, 2, 2, 3, 0, 2, 2, 4, 0, 2, 2, 5, 0,
 2, 2, 6, 0, 2, 2, 7, 0, 2, 2, 8, 0, 2, 2, 9, 0,
 2, 2, 10, 0, 2, 2, 11, 0, 2, 2, 12, 0, 2, 2, 13, 0,
 2, 24, 2, 0, 1, 0, 0, 0, 2, 4, 2, 0, 2, 5, 2, 0,
 1, 2, 0, 0, 2, 3, 2, 0, 2, 4, 2, 0, 2, 5, 2, 0,
 2, 6, 2, 0, 2, 7, 2, 0, 2, 8, 2, 0, 2, 9, 2, 0,
 2, 10, 2, 0, 2, 11, 2, 0, 2, 12, 2, 0, 2, 13, 2, 0
};


// List all types of arguments available to instructions
// Referenced by InstArgsTbl[]
#define ArgsTextTblLength 25
static char *ArgsTextTbl[] = {
 "addr11",
 "addr16",
 "A",
 "direct",
 "@R0",
 "@R1",
 "R0",
 "R1",
 "R2",
 "R3",
 "R4",
 "R5",
 "R6",
 "R7",
 "bitaddr",
 "reladdr",
 "#data",
 "C",
 "@A+DPTR",
 "@A+PC",
 "AB",
 "DPTR",
 "#data16",
 "/bitaddr",
 "@DPTR"
};




#endif
