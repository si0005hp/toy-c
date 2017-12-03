%{
  #include <stdio.h>
  #include "main.h"
  extern int yylex(void);
  extern int yyerror(char const *str);
%}
%union {
  int value;
  Node *node;
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
  | LPAREN expression RPAREN
    {
      $$ = $2;
    };
primary: INTEGER
%%