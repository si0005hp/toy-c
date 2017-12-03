%{
  #include <stdio.h>
  #include "y.tab.h"
%}

%option noinput
%option nounput

%%
[1-9][0-9]*|0 {
  int i = atoi(yytext);
  yylval.value = i;
  return INTEGER;
}

"+"  return ADD;
"-"  return SUB;
"*"  return MUL;
"/"  return DIV;

\r\n?|\n  return CR;
[ \t] ;

. {
  printf("unknown: %s\n", yytext);
  exit(1);
}
%%