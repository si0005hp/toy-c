#ifndef TOYC_H_
#define TOYC_H_

enum {
  NODE_INTEGER,
  NODE_FLOAT,
  NODE_BINOP_ADD,
  NODE_BINOP_SUB,
  NODE_BINOP_MUL,
  NODE_BINOP_DIV
};

typedef struct Node {
  int type;
  union {
    // integer
    int ival;
    // float
    double fval;
    // bin operator
    struct {
      struct Node *left;
      struct Node *right;
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
  IC_DIV
};

Node* new_int_node(int i);
Node* new_float_node(double d);
Node* new_binop_node(int op, Node *left, Node *right);

void compile_node(Node *node);
void execute_code();
void print_result();
void debug_code();

#endif
