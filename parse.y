%{
  #include <stdio.h>
  extern int yylex(void);
  extern int yyerror(char const *str);
%}
%union {
  int value;
}
%token <value> INTEGER
%type <value> primary expression term
%token CR ADD SUB MUL DIV LPAREN RPAREN
%%
program: line
  | program line 
line: CR
  | expression CR
    {
      int ans = $1;
      printf("%d\n", ans);
    };
expression: term
  | expression ADD term
    {
      $$ = $1 + $3;
    };
  | expression SUB term
    {
      $$ = $1 - $3;
    };
term: primary
  | term MUL primary
    {
      $$ = $1 * $3;
    }; 
  | term DIV primary
    {
      $$ = $1 / $3;
    }; 
  | LPAREN term RPAREN
    {
      $$ = $2;
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