#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "toyc.h"
#include "y.tab.h"

int yyerror(State *s, char const *str);
int new_var(char *varname, int type);
int idx_var(char *varname);
int cnt_var(int type);
Env resolve_var(char *varname);
int new_label(char *name, int addr);
int resolve_addr(char *name);

int main(int argc, char **argv) {
  if (argc > 1)
    stdin = fopen(argv[1], "rb");

  State *s = new_state();
  if (yyparse(s)) {
    exit(1);
  }
  compile_node(s->node);
  execute_code();
  return 0;
}

int yyerror(State *s, char const *str) {
  fprintf(stderr, "%s\n", str);
  return 0;
}

State* new_state() {
  State *s = malloc(sizeof(State));
  s->node = NULL;
  return s;
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

Node* new_lvar_node(Node *idt) {
  Node *node = malloc(sizeof(Node));
  node->type = NODE_LVAR;
  node->lname = idt->idtname;
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

void reverse_nodes(Node *nodes) {
  Node *_n = nodes;
  Node **rev_nodes = malloc(sizeof(Node*) * _n->len);
  for (int i = 0; i < _n->len; i++)
    rev_nodes[_n->len - 1 - i] = _n->nodes[i];
  _n->nodes = rev_nodes;
}

Node* new_nodes() {
  Node *node = malloc(sizeof(Node));
  node->type = NODE_NODES;
  node->len = 0;
  node->max = 0;
  return node;
}

Node* new_funcdef_node(Node *idt, Node *params, Node *block) {
  Node *node = malloc(sizeof(Node));
  node->type = NODE_FUNC_DEF;
  node->fname = idt->idtname;
  node->params = params;
  node->block = block;
  return node;
}

Node* new_funccall_node(Node *idt, Node *args) {
  Node *node = malloc(sizeof(Node));
  node->type = NODE_FUNC_CALL;
  node->fname = idt->idtname;
  node->args = args;
  return node;
}

Node* new_funcparam_node(Node *idt) {
  Node *node = malloc(sizeof(Node));
  node->type = NODE_FUNC_PARAM;
  node->pname = idt->idtname;
  return node;
}

Node* new_return_node(Node *retval) {
  Node *node = malloc(sizeof(Node));
  node->type = NODE_RETURN;
  node->retval = retval;
  return node;
}

ICode iCodes[100];
int ic_idx;

double stack[200];

Env env[200];
int e_idx;

Label labels[100];
int l_idx;

void compile_node(Node *n) {
  switch (n->type) {
    case NODE_INTEGER:
      iCodes[ic_idx].opcode = IC_PUSH;
      iCodes[ic_idx].operand = n->ival;
      ic_idx++;
      break;
    case NODE_FLOAT:
      iCodes[ic_idx].opcode = IC_PUSH;
      iCodes[ic_idx].operand = n->fval;
      ic_idx++;
      break;
    case NODE_BINOP_ADD:
      compile_node(n->left);
      compile_node(n->right);
      iCodes[ic_idx].opcode = IC_ADD;
      ic_idx++;
      break;
    case NODE_BINOP_SUB:
      compile_node(n->left);
      compile_node(n->right);
      iCodes[ic_idx].opcode = IC_SUB;
      ic_idx++;
      break;
    case NODE_BINOP_MUL:
      compile_node(n->left);
      compile_node(n->right);
      iCodes[ic_idx].opcode = IC_MUL;
      ic_idx++;
      break;
    case NODE_BINOP_DIV:
      compile_node(n->left);
      compile_node(n->right);
      iCodes[ic_idx].opcode = IC_DIV;
      ic_idx++;
      break;
    case NODE_IDT: {
      Env e = resolve_var(n->idtname);
      if (e.type == VAR_LOCAL)
        iCodes[ic_idx].opcode = IC_LOADL;
      else if (e.type == VAR_ARG)
        iCodes[ic_idx].opcode = IC_LOADA;
      iCodes[ic_idx].operand = e.idx;
      ic_idx++;
      break;
    }
    case NODE_LVAR:
      new_var(n->lname, VAR_LOCAL);
      break;
    case NODE_LET:
      compile_node(n->right);
      Env e = resolve_var(n->left->idtname);
      if (e.type == VAR_LOCAL)
        iCodes[ic_idx].opcode = IC_STOREL;
      else if (e.type == VAR_ARG)
        iCodes[ic_idx].opcode = IC_STOREA;
      iCodes[ic_idx].operand = e.idx;
      ic_idx++;
      break;
    case NODE_INIT:
      compile_node(n->right);
      iCodes[ic_idx].opcode = IC_STOREL;
      iCodes[ic_idx].operand = new_var(n->left->idtname, VAR_LOCAL);
      ic_idx++;
      break;
    case NODE_PRINT:
      switch (n->target->type) {
        case NODE_IDT:
          iCodes[ic_idx].opcode = IC_PRINT;
          iCodes[ic_idx].operand = idx_var(n->target->idtname);
          ic_idx++;
          break;
        default:
          compile_node(n->target);
          iCodes[ic_idx].opcode = IC_PRINT;
          iCodes[ic_idx].operand = -1;
          ic_idx++;
          break;
      }
      break;
    case NODE_NODES:
      for (int i = 0; i < n->len; i++)
        compile_node(n->nodes[i]);
      break;
    case NODE_FUNC_DEF: {
      int ic_idx_save = ic_idx;
      ic_idx += 2;

      e_idx = 0;
      compile_node(n->params);
      compile_node(n->block);

      iCodes[ic_idx_save].opcode = IC_ENTRY;
      new_label(n->fname, ic_idx_save);
      iCodes[ic_idx_save + 1].opcode = IC_FRAME;
      iCodes[ic_idx_save + 1].operand = cnt_var(VAR_LOCAL);
      break;
    }
    case NODE_FUNC_PARAM:
      new_var(n->pname, VAR_ARG);
      break;
    case NODE_FUNC_CALL:
      reverse_nodes(n->args); // To correct args order
      compile_node(n->args);
      iCodes[ic_idx].opcode = IC_CALL;
      iCodes[ic_idx].operand = resolve_addr(n->fname);
      ic_idx++;
      iCodes[ic_idx].opcode = IC_POPR;
      iCodes[ic_idx].operand = n->args->len;
      ic_idx++;
      break;
    case NODE_RETURN:
      compile_node(n->retval);
      iCodes[ic_idx].opcode = IC_RET;
      ic_idx++;
      break;
    default:
      fprintf(stderr, "Failed to compile_node by illegal node type: %d\n",
          n->type);
      exit(1);
  }
}

int new_var(char *varname, int type) {
  for (int i = 0; i <= e_idx - 1; i++) {
    if (strcmp(env[i].varname, varname) == 0) {
      fprintf(stderr, "Duplicate variable: %s\n", varname);
      exit(1);
    }
  }
  env[e_idx].varname = varname;
  env[e_idx].type = type;
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

int cnt_var(int type) {
  int cnt = 0;
  for (int i = 0; i <= e_idx - 1; i++) {
    if (env[i].type == type)
      cnt++;
  }
  return cnt;
}

Env resolve_var(char *varname) {
  for (int i = e_idx - 1; i >= 0; i--) {
    if (strcmp(env[i].varname, varname) == 0)
      return env[i];
  }
  fprintf(stderr, "Not found variable: %s\n", varname);
  exit(1);
}

int new_label(char *name, int addr) {
  labels[l_idx].name = name;
  labels[l_idx].addr = addr;
  return l_idx++;
}

int resolve_addr(char *name) {
  for (int i = 0; i < l_idx; i++) {
    if (strcmp(labels[i].name, name) == 0)
      return labels[i].addr;
  }
  fprintf(stderr, "Not found entry point: %s\n", name);
  exit(1);
}

void execute_code() {
  int pc, sp, fp;

  sp = 0;
  pc = resolve_addr("main");

  double x, y;
  for (;;) {
    switch (iCodes[pc].opcode) {
      case IC_PUSH:
        stack[sp++] = iCodes[pc].operand;
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
      case IC_STOREL:
        y = stack[--sp];
        stack[fp + (int) iCodes[pc].operand] = y;
        break;
      case IC_STOREA:
        y = stack[--sp];
        stack[fp - (int) iCodes[pc].operand - 3] = y;
        break;
      case IC_LOADL:
        y = stack[fp + (int) iCodes[pc].operand];
        stack[sp++] = y;
        break;
      case IC_LOADA:
        y = stack[fp - (int) iCodes[pc].operand - 3];
        stack[sp++] = y;
        break;
      case IC_PRINT:
        if (iCodes[pc].operand == -1) {
          y = stack[--sp];
        } else {
          y = stack[fp + (int) iCodes[pc].operand];
        }
        printf("%g\n", y);
        break;
      case IC_ENTRY:
        break;
      case IC_RET:
        y = stack[--sp];
        sp = fp;
        fp = stack[--sp];
        if (sp <= 0) {
          return;
        }
        pc = stack[--sp];
        continue;
      case IC_FRAME:
        stack[sp++] = fp;
        fp = sp;
        sp += iCodes[pc].operand;
        break;
      case IC_CALL:
        stack[sp++] = pc + 1;
        pc = iCodes[pc].operand;
        continue;
      case IC_POPR:
        sp -= iCodes[pc].operand;
        stack[sp++] = y;
        break;
      default:
        fprintf(stderr, "Unknown opcode: %d\n", iCodes[pc].opcode);
        exit(1);
    }
    pc++;
    if (pc > ic_idx) {
      break;
    }
  }
}

void debug_code() {
  for (int i = 0; i < ic_idx; i++) {
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
        opcode = "IC_STOREL";
        break;
      case 6:
        opcode = "IC_LOADL";
        break;
      case 7:
        opcode = "IC_PRINT";
        break;
      case 8:
        opcode = "IC_ENTRY";
        break;
      case 9:
        opcode = "IC_RET";
        break;
      case 10:
        opcode = "IC_FRAME";
        break;
      case 11:
        opcode = "IC_CALL";
        break;
      case 12:
        opcode = "IC_POPR";
        break;
      case 13:
        opcode = "IC_STOREA";
        break;
      case 14:
        opcode = "IC_LOADA";
        break;
    }
    printf("opcode: [%-10s], operand:%g \n", opcode, iCodes[i].operand);
  }
}
