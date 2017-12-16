#ifndef TOYC_H_
#define TOYC_H_

enum {
  NODE_INTEGER,
  NODE_FLOAT,
  NODE_BINOP_ADD,
  NODE_BINOP_SUB,
  NODE_BINOP_MUL,
  NODE_BINOP_DIV,
  NODE_IDT,
  NODE_INIT,
  NODE_PRINT, // Temporary
  NODE_NODES
};

typedef struct Node {
  int type;
  union {
    // integer
    int ival;
    // float
    double fval;
    // binop, let
    struct {
      struct Node *left;
      struct Node *right;
    };
    // identifier
    char *idtname;
    // print (Temporary)
    struct Node *target;
    // nodes
    struct {
      int len;
      int max;
      struct Node **nodes;
    };
  };
} Node;

typedef struct ICode {
  int opcode;
  double operand;
} ICode;

enum {
  IC_PUSH,
  IC_ADD,
  IC_SUB,
  IC_MUL,
  IC_DIV,
  IC_STOREV,
  IC_PRINT, // Temporary
};

typedef struct Env {
  char *varname;
  int idx;
} Env;

Node* new_int_node(int i);
Node* new_float_node(double d);
Node* new_binop_node(int op, Node *left, Node *right);
Node* new_idt_node(char *idtname);
Node* new_init_node(Node *left, Node *right);
Node* new_print_node(Node *target); // Temporary
void append_nodes(Node *nodes, Node *node);

void compile_node(Node *node);
void execute_code();
void print_result();
void debug_code();

#endif
