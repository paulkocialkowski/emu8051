%option noyywrap
%option noinput
%option nounput
%{
  #include "menu.h"
  #include "parser.h"
  #include "hexfile.h"
  #include "options.h"

#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) result = menu_get_input(buf, max_size);

%}

digit		[0-9]
alpha		[a-fA-F]
hextail		({digit}|{alpha}){1,8}
hex1		0[xX]{hextail}
hex2		${hextail}


%%
{hex1}      {
                /*
                 * No need to check return value of asciihex2int, because lex
                 * always passes a valid ASCII hex string.
                 */
                yylval.number = asciihex2int(&yytext[2]); /* Skip "0x" prefix */
                return NUMBER;
            }
{hex2}      {
                /*
                 * No need to check return value of asciihex2int, because lex
                 * always passes a valid ASCII hex string.
                 */
                yylval.number = asciihex2int(&yytext[1]); /* Skip "$" prefix */
                return NUMBER;
            }


[0-9]+      { yylval.number = atoi(yytext); return NUMBER; }
[h?]        return TOK_HELP;
sb          return TOK_SB;
rb          return TOK_RB;
db          return TOK_DB;
de          return TOK_DE;
di          return TOK_DI;
dp          return TOK_DP;
dr          return TOK_DR;
r           return TOK_RUN;
q           return TOK_QUIT;
s           return TOK_STEP;
u           return TOK_UNASM;
we          return TOK_MOD_EXT;
wi          return TOK_MOD_INT;
wp          return TOK_MOD_PROG;
wr          return TOK_MOD_REG;
x           return TOK_START_TRACING;
X           return TOK_STOP_TRACING;
z           return TOK_RST;
zt          return TOK_RST_TIMER;
all         return TOK_ALL;
a           return TOK_A;
b           return TOK_B;
c           return TOK_C;
d           return TOK_D;
[a-z0-9]+   { yylval.string = strdup(yytext); return WORD; }
[\n]        return TOK_ENTER;
[ \t]+      { /* ignore whitespace */ }
.           { return yytext[0]; }
%%
