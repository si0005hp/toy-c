%{
  #include <stdio.h>
  #include "toyc.h"
  #include "y.tab.h"
  extern int fileno(FILE *);
%}

%option noinput
%option nounput

%%
"+"  return ADD;
"-"  return SUB;
"*"  return MUL;
"/"  return DIV;
"("  return LPAREN;
")"  return RPAREN;

"print"  return PRINT; // Temporary

\r\n?|\n  return CR;
[ \t] ;

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

. {
  printf("unknown: %s\n", yytext);
  exit(1);
}
%%