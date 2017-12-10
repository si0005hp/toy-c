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

Node* new_idt_node(char *idtname) {
  Node *node = malloc(sizeof(Node));
  node->type = NODE_IDT;
  node->idtname = idtname;
  return node;
}

ICode iCodes[100];
int ic_idx;

double stack[200];
int sp;

void compile_node(Node *n) {
  switch (n->type) {
    case NODE_INTEGER:
      iCodes[ic_idx].opcode = IC_PUSH;
      iCodes[ic_idx].operand = n->ival;
      break;
    case NODE_FLOAT:
      iCodes[ic_idx].opcode = IC_PUSH;
      iCodes[ic_idx].operand = n->fval;
      break;
    case NODE_BINOP_ADD:
      compile_node(n->left);
      compile_node(n->right);
      iCodes[ic_idx].opcode = IC_ADD;
      break;
    case NODE_BINOP_SUB:
      compile_node(n->left);
      compile_node(n->right);
      iCodes[ic_idx].opcode = IC_SUB;
      break;
    case NODE_BINOP_MUL:
      compile_node(n->left);
      compile_node(n->right);
      iCodes[ic_idx].opcode = IC_MUL;
      break;
    case NODE_BINOP_DIV:
      compile_node(n->left);
      compile_node(n->right);
      iCodes[ic_idx].opcode = IC_DIV;
      break;
    default:
      fprintf(stderr, "Failed to compile_node by illegal node type: %d\n",
          n->type);
      exit(1);
  }
  ++ic_idx;
}

void execute_code() {
  double x, y;
  for (int i = 0; i < ic_idx; i++) {
    switch (iCodes[i].opcode) {
      case IC_PUSH:
        stack[sp++] = iCodes[i].operand;
        break;
      case IC_ADD:
        y = stack[--sp];
        x = stack[--sp];
        stack[sp++] = x + y;
        break;
      case IC_SUB:
        y = stack[--sp];
        x = stack[--sp];
        stack[sp++] = x - y;
        break;
      case IC_MUL:
        y = stack[--sp];
        x = stack[--sp];
        stack[sp++] = x * y;
        break;
      case IC_DIV:
        y = stack[--sp];
        x = stack[--sp];
        stack[sp++] = x / y;
        break;
    }
  }
}

void print_result() {
  printf("%g\n", stack[--sp]); // Print popped stack element as calculated result
}

void debug_code() {
  for (int i = 0; i < ic_idx; i++) {
    printf("opcode: %i, operand:%g \n", iCodes[i].opcode, iCodes[i].operand);
  }
}
