%{
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "log.h"
#include "menu.h"
#include "memory.h"
#include "timers.h"
#include "cpu8051.h"

/* int yydebug = 1; */

/* To get rid of compiler warning. */
int yylex();

int yyerror(const char *str)
{
	fprintf(stderr,"error: %s\n", str);
        return 0;
}

%}

%token NUMBER TOK_ENTER TOK_ALL
%token TOK_SB TOK_RB TOK_DB
%token TOK_DE TOK_DI TOK_DP TOK_DR
%token TOK_PC
%token TOK_HELP
%token TOK_RUN
%token TOK_RST TOK_RST_TIMER
%token TOK_STEP
%token TOK_UNASM
%token TOK_MOD_EXT TOK_MOD_INT TOK_MOD_PROG TOK_MOD_REG
%token TOK_QUIT

%%

start  :  start command { menu_prompt(); }
   |  error TOK_ENTER {
         /* In case of error, discard entire line */
         yyerrok;
         menu_prompt();
      }
   | start TOK_ENTER { menu_prompt(); }
   |
   ;

command:
   pc_set
   |
   breakpoint_clr
   |
   breakpoint_set
   |
   breakpoint_display
   |
   dump_ext_mem
   |
   dump_int_mem
   |
   dump_prog_mem
   |
   display_regs
   |
   help
   |
   modify
   |
   quit
   |
   reset
   |
   run
   |
   step
   |
   unasm
   ;

breakpoint_clr:
	TOK_RB NUMBER TOK_ENTER
	{
          log_debug("  Remove breakpoint at $%04X", $2);
          ClearBreakpoint($2);
	}
        |
	TOK_RB TOK_ENTER
	{
          log_debug("  Remove breakpoint at PC");
          ClearBreakpoint(cpu8051.pc);
	}
        |
	TOK_RB TOK_ALL TOK_ENTER
	{
          log_debug("  Remove all breakpoints");
          ClearAllBreakpoints();
	}
	;

breakpoint_set:
	TOK_SB TOK_ENTER
	{
          log_debug("  Set breakpoint at current PC");
          SetBreakpoint(cpu8051.pc);
	}
        |

	TOK_SB NUMBER TOK_ENTER
	{
          log_debug("  Set breakpoint at $%04X", $2);
          SetBreakpoint($2);
	}
	;

breakpoint_display:
	TOK_DB TOK_ENTER
	{
          log_debug("  Display breakpoints");
          ShowBreakpoints();
	}
	;

dump_ext_mem:
	TOK_DE NUMBER NUMBER TOK_ENTER
	{
          log_debug("  Dump External Data Memory at $%04X, len %d", $2, $3);
          DumpMem($2, $3, EXT_MEM_ID);
	}
	;

dump_int_mem:
	TOK_DI NUMBER NUMBER TOK_ENTER
	{
          log_debug("  Dump Internal Data Memory at $%04X, len %d", $2, $3);
          DumpMem($2, $3, INT_MEM_ID);
	}
	;

dump_prog_mem:
	TOK_DP NUMBER NUMBER TOK_ENTER
	{
          log_debug("  Dump Program Memory at $%04X, len %d", $2, $3);
          DumpMem($2, $3, PGM_MEM_ID);
	}
	;

display_regs:
	TOK_DR TOK_ENTER
	{
          log_debug("  Display Registers");
          console_show_registers();
	}
	;

modify:
	TOK_MOD_EXT NUMBER NUMBER TOK_ENTER
	{
          log_debug("  Modify external memory");
          memory_write8(EXT_MEM_ID, $2, $3);
	}
	|
	TOK_MOD_INT NUMBER NUMBER TOK_ENTER
	{
          log_debug("  Modify internal memory");
          memory_write8(INT_MEM_ID, $2, $3);
	}
	|
	TOK_MOD_PROG NUMBER NUMBER TOK_ENTER
	{
          log_debug("  Modify program memory");
          memory_write8(PGM_MEM_ID, $2, $3);
	}
	|
	TOK_MOD_REG "pc" NUMBER TOK_ENTER
	{
          log_debug("  Modify register");
          SetRegister("PC", $2);
	}
	;

quit:
	TOK_QUIT TOK_ENTER
	{
          printf("  Quit");
          YYACCEPT;
	}
	;

run:
	TOK_RUN TOK_ENTER
	{
          log_debug("  Run");
          console_exec(-1);
	}
        |
	TOK_RUN NUMBER TOK_ENTER
	{
          log_debug("  Run %d instructions", $2);
          console_exec($2);
	}
	;

pc_set:
	TOK_PC NUMBER TOK_ENTER
	{
          cpu8051.pc = $2;
	}
	;

help:
	TOK_HELP TOK_ENTER
	{
          menu_display_usage();
	}
	;

reset:
	TOK_RST TOK_ENTER
	{
          cpu8051_Reset();
	}
        |
	TOK_RST_TIMER TOK_ENTER
	{
          gp_timer_reset();
	}
        ;

step:
	TOK_STEP TOK_ENTER
	{
          console_trace();
	}
        ;

unasm:
	TOK_UNASM NUMBER NUMBER TOK_ENTER
	{
          DisasmN($2, $3);
	}
        |
	TOK_UNASM NUMBER TOK_ENTER
	{
          DisasmN(cpu8051.pc, $2);
	}
	;
        |
	TOK_UNASM TOK_ENTER
	{
          DisasmN(cpu8051.pc, 16);
	}
	;

