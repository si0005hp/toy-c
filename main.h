#ifndef MAIN_H_
#define MAIN_H_

enum {
  NODE_INTEGER,
  NODE_BINOP_ADD,
  NODE_BINOP_SUB,
  NODE_BINOP_MUL,
  NODE_BINOP_DIV
};

typedef struct Node {
  int type;
  union {
    // integer
    int value;
    // bin operator
    struct {
      struct Node *left;
      struct Node *right;
    };
  };
} Node;

Node* new_int_node(int i);
Node* new_binop_node(int op, Node *left, Node *right);

int expr_node(Node *node);

#endif
