%option noyywrap
%option noinput
%option nounput
%{
  #include "parser.h"
%}
%%
[0-9]+      { yylval = atoi(yytext); return NUMBER;}
[h?]        return TOK_HELP;
sb          return TOK_SB;
rb          return TOK_RB;
db          return TOK_DB;
de          return TOK_DE;
di          return TOK_DI;
dp          return TOK_DP;
dr          return TOK_DR;
r           return TOK_RUN;
pc          return TOK_PC;
all         return TOK_ALL;
me          return TOK_MOD_EXT;
mi          return TOK_MOD_INT;
mp          return TOK_MOD_PROG;
mr          return TOK_MOD_REG;
q           return TOK_QUIT;
s           return TOK_STEP;
u           return TOK_UNASM;
z           return TOK_RST;
zt          return TOK_RST_TIMER;
[\n]        return TOK_ENTER;
[ \t]+      { /* ignore whitespace */ }
.           { return yytext[0];}
%%
