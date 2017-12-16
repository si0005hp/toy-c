#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "toyc.h"
#include "y.tab.h"

int yyerror(char const *str);
int new_var(char *varname);
int idx_var(char *varname);
void reserve_sp(int e_idx);

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

Node* new_let_node(Node *left, Node *right) {
  Node *node = malloc(sizeof(Node));
  node->type = NODE_LET;
  node->left = left;
  node->right = right;
  return node;
}

Node* new_init_node(Node *left, Node *right) {
  Node *node = malloc(sizeof(Node));
  node->type = NODE_INIT;
  node->left = left;
  node->right = right;
  return node;
}

Node* new_print_node(Node *target) {
  Node *node = malloc(sizeof(Node));
  node->type = NODE_PRINT;
  node->target = target;
  return node;
}

void append_nodes(Node *nodes, Node *node) {
  Node *_n = nodes;
  if (_n->len == _n->max) {
    _n->max += 10;
    _n->nodes = realloc(_n->nodes, sizeof(Node*) * _n->max);
  }
  _n->nodes[_n->len] = node;
  _n->len++;
}

Node* new_nodes() {
  Node *node = malloc(sizeof(Node));
  node->type = NODE_NODES;
  node->len = 0;
  node->max = 0;
  return node;
}

ICode iCodes[100];
int ic_idx;

double stack[200];
int sp;

Env env[200];
int e_idx;

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
    case NODE_IDT:
      iCodes[ic_idx].opcode = IC_LOADV;
      iCodes[ic_idx].operand = idx_var(n->idtname);
      break;
    case NODE_LET:
      compile_node(n->right);
      iCodes[ic_idx].opcode = IC_STOREV;
      iCodes[ic_idx].operand = idx_var(n->left->idtname);
      break;
    case NODE_INIT:
      compile_node(n->right);
      iCodes[ic_idx].opcode = IC_STOREV;
      iCodes[ic_idx].operand = new_var(n->left->idtname);
      break;
    case NODE_PRINT:
      switch (n->target->type) {
        case NODE_IDT:
          iCodes[ic_idx].opcode = IC_PRINT;
          iCodes[ic_idx].operand = idx_var(n->target->idtname);
          break;
        default:
          compile_node(n->target);
          iCodes[ic_idx].opcode = IC_PRINT;
          iCodes[ic_idx].operand = -1;
          break;
      }
      break;
    case NODE_NODES:
      for (int i = 0; i < n->len; i++)
        compile_node(n->nodes[i]);
      break;
    default:
      fprintf(stderr, "Failed to compile_node by illegal node type: %d\n",
          n->type);
      exit(1);
  }
  ++ic_idx;
}

int new_var(char *varname) {
  env[e_idx].varname = varname;
  env[e_idx].idx = e_idx;
  return e_idx++;
}

int idx_var(char *varname) {
  for (int i = e_idx - 1; i >= 0; i--) {
    if (strcmp(env[i].varname, varname) == 0)
      return env[i].idx;
  }
  return -1;
}

void reserve_sp(int e_idx) {
  for (int i = 0; i < e_idx; i++) {
    stack[sp++] = 0;
  }
}

void execute_code() {
  reserve_sp(e_idx);
  double x, y;
  for (int i = 0; i < ic_idx - 1; i++) {
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
      case IC_STOREV:
        y = stack[--sp];
        stack[(int) iCodes[i].operand] = y;
        break;
      case IC_LOADV:
        y = stack[(int) iCodes[i].operand];
        stack[sp++] = y;
        break;
      case IC_PRINT:
        if (iCodes[i].operand == -1) {
          y = stack[--sp];
        } else {
          y = stack[(int) iCodes[i].operand];
        }
        printf("%g\n", y);
        break;
    }
  }
}

void print_result() {
  printf("%g\n", stack[--sp]); // Print popped stack element as calculated result
}

void debug_code() {
  for (int i = 0; i < ic_idx - 1; i++) {
    char *opcode;
    switch (iCodes[i].opcode) {
      case 0:
        opcode = "IC_PUSH";
        break;
      case 1:
        opcode = "IC_ADD";
        break;
      case 2:
        opcode = "IC_SUB";
        break;
      case 3:
        opcode = "IC_MUL";
        break;
      case 4:
        opcode = "IC_DIV";
        break;
      case 5:
        opcode = "IC_STOREV";
        break;
      case 6:
        opcode = "IC_PRINT";
        break;
    }
    printf("opcode: [%-10s], operand:%g \n", opcode, iCodes[i].operand);
  }
}
