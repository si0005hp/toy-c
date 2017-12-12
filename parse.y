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
%type <node> INTEGER FLOAT IDENTIFIER primary expression term statement print_statement print_var_statement program
%token CR INT ADD SUB MUL DIV LPAREN RPAREN SEMICOLON EQ IDENTIFIER INTEGER FLOAT
%token PRINT
%%
program: statement print_var_statement
    {
      compile_node($1);
      compile_node($2);
      execute_code();
      exit(0);
    };
  | print_statement
    {
      compile_node($1);
      execute_code();
      exit(0);
    }
statement: INT IDENTIFIER SEMICOLON CR
    {
    }
  | INT IDENTIFIER EQ expression SEMICOLON CR
    {
      $$ = new_init_node($2, $4);
    }
print_statement: PRINT expression CR
    {
      $$ = new_print_node($2);
    }
print_var_statement: PRINT IDENTIFIER CR
    {
      $$ = new_print_node($2);
    }
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