%{
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "log.h"
#include "menu.h"
#include "memory.h"
#include "gp_timers.h"
#include "memory.h"
#include "cpu8051.h"
#include "options.h"

/* int yydebug = 1; */

/* To get rid of compiler warning. */
int yylex();

int yyerror(const char *str)
{
        fprintf(stderr,"error: %s\n", str);
        return 0;
}

%}

%union
{
        int number;
        char *string;
}

%token <number> NUMBER
%token <string> WORD
%token TOK_ENTER TOK_ALL
%token TOK_SB TOK_RB TOK_DB
%token TOK_DE TOK_DI TOK_DP TOK_DR
%token TOK_HELP
%token TOK_RUN
%token TOK_RST TOK_RST_TIMER
%token TOK_STEP
%token TOK_UNASM
%token TOK_MOD_EXT TOK_MOD_INT TOK_MOD_PROG TOK_MOD_REG
%token TOK_START_TRACING TOK_STOP_TRACING
%token TOK_QUIT
%token TOK_A TOK_B TOK_C TOK_D

%%

start  :  start command
   |  error TOK_ENTER {
         /* In case of error, discard entire line */
         yyerrok;
      }
   | start TOK_ENTER
   |
   ;

command:
   breakpoint_clr
   |
   breakpoint_set
   |
   breakpoint_display
   |
   dump_mem
   |
   display_regs
   |
   help
   |
   modify
   |
   tracing
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
          breakpoint_clr($2);
	}
        |
	TOK_RB TOK_ENTER
	{
          log_debug("  Remove breakpoint at PC");
          breakpoint_clr(cpu8051.pc);
	}
        |
	TOK_RB TOK_ALL TOK_ENTER
	{
          log_debug("  Remove all breakpoints");
          breakpoints_clr_all();
	}
	;

breakpoint_set:
	TOK_SB TOK_ENTER
	{
          log_debug("  Set breakpoint at PC");
          breakpoint_set(cpu8051.pc);
	}
        |

	TOK_SB NUMBER TOK_ENTER
	{
          log_debug("  Set breakpoint at $%04X", $2);
          breakpoint_set($2);
        }
	;

breakpoint_display:
	TOK_DB TOK_ENTER
	{
          log_debug("  Display breakpoints");
          breakpoints_show();
	}
	;

dump_mem:
	TOK_DE NUMBER NUMBER TOK_ENTER
	{
          log_debug("  Dump External Data Memory at $%04X, len %d", $2, $3);
          mem_dump($2, $3, EXT_MEM_ID);
	}
        |
	TOK_DI NUMBER NUMBER TOK_ENTER
	{
          log_debug("  Dump Internal Data Memory at $%04X, len %d", $2, $3);
          mem_dump($2, $3, INT_MEM_ID);
	}
        |
	TOK_DP NUMBER NUMBER TOK_ENTER
	{
          log_debug("  Dump Program Memory at $%04X, len %d", $2, $3);
          mem_dump($2, $3, PGM_MEM_ID);
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
          mem_write8(EXT_MEM_ID, $2, $3, false);
	}
	|
	TOK_MOD_INT NUMBER NUMBER TOK_ENTER
	{
          log_debug("  Modify internal memory");
          mem_write8(INT_MEM_ID, $2, $3, false);
	}
	|
	TOK_MOD_PROG NUMBER NUMBER TOK_ENTER
	{
          log_debug("  Modify program memory");
          mem_write8(PGM_MEM_ID, $2, $3, false);
	}
	|
	TOK_MOD_REG WORD NUMBER TOK_ENTER
	{
          register_set($2, $3);
	}
	;

tracing:
	TOK_START_TRACING TOK_ENTER
	{
          printf("  Starting trace\n");
          options.tracing = 1;
	}
	|
	TOK_STOP_TRACING TOK_ENTER
	{
          printf("  Stopping trace\n");
          options.tracing = 0;
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

help:
	TOK_HELP TOK_ENTER
	{
          menu_display_usage();
	}
	;

reset:
	TOK_RST TOK_ENTER
	{
          cpu8051_reset();
	}
        |
	TOK_RST_TIMER TOK_A TOK_ENTER
	{
          gp_timer_reset(0);
	}
        |
	TOK_RST_TIMER TOK_B TOK_ENTER
	{
          gp_timer_reset(1);
	}
        |
	TOK_RST_TIMER TOK_C TOK_ENTER
	{
          gp_timer_reset(2);
	}
        |
	TOK_RST_TIMER TOK_D TOK_ENTER
	{
          gp_timer_reset(3);
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
          disassemble_num($2, $3);
	}
        |
	TOK_UNASM NUMBER TOK_ENTER
	{
          disassemble_num(cpu8051.pc, $2);
	}
	;
        |
	TOK_UNASM TOK_ENTER
	{
          disassemble_num(cpu8051.pc, 16);
	}
	;


