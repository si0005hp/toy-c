%{
  #include <stdio.h>
  #include "toyc.h"
  extern int yylex(void);
  extern int yyerror(char const *str);
%}
%union {
  Node *node;
}
%type <node> INTEGER FLOAT primary expression term
%token CR ADD SUB MUL DIV LPAREN RPAREN INTEGER FLOAT
%%
program: line
  | program line
line: CR
  | expression CR
    {
      double ans = expr_node($1);
      printf("%g\n", ans);
    }; 
expression: term
  | expression ADD term
    {
      $$ = new_binop_node(NODE_BINOP_ADD, $1, $3);
    };
  | expression SUB term
    {
      $$ = new_binop_node(NODE_BINOP_SUB, $1, $3);
    };
term: primary
  | term MUL primary
    {
      $$ = new_binop_node(NODE_BINOP_MUL, $1, $3);
    }; 
  | term DIV primary
    {
      $$ = new_binop_node(NODE_BINOP_DIV, $1, $3);
    };
  | LPAREN expression RPAREN
    {
      $$ = $2;
    };
primary: INTEGER
  | FLOAT
%%