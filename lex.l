%{
  #include <stdio.h>
  #include "toyc.h"
  #include "y.tab.h"
  extern int fileno(FILE *);
%}

%option noinput
%option nounput

%%
"int" return INT;
"return" return RETURN;

"+"  return ADD;
"-"  return SUB;
"*"  return MUL;
"/"  return DIV;
"("  return LPAREN;
")"  return RPAREN;
"{"  return LC;
"}"  return RC;
";"  return SEMICOLON;
"="  return EQ;
","  return COMMA;

"print"  return PRINT; // Temporary

\r\n?|\n ;
[ \t] ;

[A-Za-z_][A-Za-z_0-9]* {
  char* s = malloc(yyleng + 1); // TODO: free
  strcpy(s, yytext);
  yylval.node = new_idt_node(s);
  return IDENTIFIER;
}
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