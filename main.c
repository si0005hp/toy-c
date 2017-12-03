#include <stdio.h>
#include <stdlib.h>
#include "toyc.h"
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
  node->ival = i;
  return node;
}

Node* new_float_node(double d) {
  Node *node = malloc(sizeof(Node));
  node->type = NODE_FLOAT;
  node->fval = d;
  return node;
}

Node* new_binop_node(int op, Node *left, Node *right) {
  Node *node = malloc(sizeof(Node));
  node->type = op;
  node->left = left;
  node->right = right;
  return node;
}

double expr_node(Node *n) {
  switch (n->type) {
    case NODE_INTEGER:
      return n->ival;
    case NODE_FLOAT:
      return n->fval;
    case NODE_BINOP_ADD:
      return expr_node(n->left) + expr_node(n->right);
    case NODE_BINOP_SUB:
      return expr_node(n->left) - expr_node(n->right);
    case NODE_BINOP_MUL:
      return expr_node(n->left) * expr_node(n->right);
    case NODE_BINOP_DIV:
      return expr_node(n->left) / expr_node(n->right);
    default:
      fprintf(stderr, "Failed expr_node by illegal node type: %d\n", n->type);
      exit(1);
  }
}
