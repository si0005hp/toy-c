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
. {
  printf("NG\n");
}
%%