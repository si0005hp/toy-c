%{
  #include <stdio.h>
  extern int yylex(void);
  extern int yyerror(char const *str);
%}
%union {
  int value;
}
%token <value> INTEGER
%type <value> factor
%token CR
%%
program: line
  | program line 
line: CR
  | factor CR;
factor: INTEGER { printf("Value: %d\n", $1); };
%%
int yyerror(char const *str) {
  fprintf(stderr, "%s\n", str);
  return 0;
}
int main(void) {
  yyparse();
  return 0;
}