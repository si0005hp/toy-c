%{
  #include <stdio.h>
  extern int yylex(void);
  extern int yyerror(char const *str);
%}
%union {
  int value;
}
%token <value> INTEGER
%type <value> primary expression
%token CR ADD SUB
%%
program: line
  | program line 
line: CR
  | expression CR
    {
      int ans = $1;
      printf("%d\n", ans);
    };
expression: primary
  | expression ADD primary
    {
      $$ = $1 + $3;
    };
primary: INTEGER
%%
int yyerror(char const *str) {
  fprintf(stderr, "%s\n", str);
  return 0;
}
int main(void) {
  yyparse();
  return 0;
}