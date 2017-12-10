%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "toyc.h"
  extern int yylex(void);
  extern int yyerror(char const *str);
%}
%union {
  Node *node;
}
%type <node> INTEGER FLOAT primary expression term statement program
%token CR ADD SUB MUL DIV LPAREN RPAREN INTEGER FLOAT
%token PRINT
%%
program: statement print_statement
    {
      Node *node = $1;
      compile_node(node);
      execute_code();
      print_result();
      exit(0);
    };
statement: expression CR
print_statement: PRINT CR

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
primary: INTEGER
  | FLOAT
  | LPAREN expression RPAREN
    {
      $$ = $2;
    };
%%