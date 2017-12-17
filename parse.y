%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "toyc.h"
  extern int yylex(void);
  extern int yyerror(State *s, char const *str);
%}
%union {
  Node *node;
}
%type <node> INTEGER FLOAT IDENTIFIER primary expression term statement statements program var block func_def
%token INT ADD SUB MUL DIV LPAREN RPAREN LC RC SEMICOLON EQ IDENTIFIER INTEGER FLOAT
%token PRINT

%parse-param {State *s}
%%
program: func_def
    {
      s->node = $1;
    };
func_def: INT IDENTIFIER LPAREN RPAREN block
    {
      $$ = new_funcdef_node($2, $5);
    }
statements: statement
    {
      $$ = new_nodes();
      append_nodes($$, $1);
    }
  | statements statement
    {
      $$ = $1;
      append_nodes($$, $2);
    }
statement: INT var SEMICOLON
    {
      $$ = $2;
    }
  | var EQ expression SEMICOLON
    {
      $$ = new_let_node($1, $3);
    }
  | INT var EQ expression SEMICOLON
    {
      $$ = new_init_node($2, $4);
    }
  | PRINT LPAREN expression RPAREN SEMICOLON
    {
      $$ = new_print_node($3);
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
var: IDENTIFIER
primary: INTEGER
  | FLOAT
  | LPAREN expression RPAREN
    {
      $$ = $2;
    };
  | var
block: LC statements RC
    {
      $$ = $2;
    }
%%