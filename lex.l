%{
  #include <stdio.h>
  #include "main.h"
  #include "y.tab.h"
%}

%option noinput
%option nounput

%%
[1-9][0-9]*|0 {
  yylval.node = new_int_node(atoi(yytext));
  return INTEGER;
}

"+"  return ADD;
"-"  return SUB;
"*"  return MUL;
"/"  return DIV;
"("  return LPAREN;
")"  return RPAREN;

\r\n?|\n  return CR;
[ \t] ;

. {
  printf("unknown: %s\n", yytext);
  exit(1);
}
%%