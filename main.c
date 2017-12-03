#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "y.tab.h"

int yyerror(char const *str);

int main(int argc, char **argv) {
  yyparse();
  return 0;
}

int yyerror(char const *str) {
  fprintf(stderr, "%s\n", str);
  return 0;
}

Node* new_int_node(int i) {
  Node *node = malloc(sizeof(Node));
  node->type = NODE_INTEGER;
  node->value = i;
  return node;
}

Node* new_binop_node(int op, Node *left, Node *right) {
  Node *node = malloc(sizeof(Node));
  node->type = op;
  node->left = left;
  node->right = right;
  return node;
}
