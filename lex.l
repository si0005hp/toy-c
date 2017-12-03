%{
  #include <stdio.h>
  #include "toyc.h"
  #include "y.tab.h"
%}

%option noinput
%option nounput

%%
[1-9][0-9]*|0 {
  yylval.node = new_int_node(atoi(yytext));
  return INTEGER;
}
(([1-9][0-9]*)|0)(\.[0-9][0-9]*)? {
  double val;
  sscanf(yytext, "%lf", &val);
  yylval.node = new_float_node(val);
  return FLOAT;
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