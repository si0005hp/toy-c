%{
  #include <stdio.h>
  #include "y.tab.h"
%}
%%
[1-9][0-9]*|0 {
  int i = atoi(yytext);
  yylval.value = i;
  return INTEGER;
}
\r\n?|\n {
  return CR;
}
. {
  printf("NG\n");
}
%%