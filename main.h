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

#endif
