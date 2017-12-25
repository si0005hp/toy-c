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
  NODE_LVAR,
  NODE_LET,
  NODE_INIT,
  NODE_PRINT, // Temporary
  NODE_NODES,
  NODE_FUNC_DEF,
  NODE_FUNC_PARAM,
  NODE_FUNC_CALL,
  NODE_RETURN
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
    // lvar
    char *lname;
    // print (Temporary)
    struct Node *target;
    // nodes
    struct {
      int len;
      int max;
      struct Node **nodes;
    };
    // funcdef or funccall
    struct {
      char *fname;
      struct Node *args;
      struct {
        struct Node *params; // nodes
        struct Node *block; // nodes
      };
    };
    // funcparam
    struct {
      char *pname;
    };
    // return
    struct Node *retval;
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
  IC_STOREL,
  IC_LOADL,
  IC_PRINT, // Temporary
  IC_ENTRY,
  IC_RET,
  IC_FRAME,
  IC_CALL,
  IC_POPR,
  IC_STOREA,
  IC_LOADA,
};

typedef struct Env {
  int type;
  char *varname;
  int idx;
} Env;

enum {
  VAR_LOCAL,
  VAR_ARG,
};

typedef struct Label {
  char *name;
  int addr;
} Label;

typedef struct State {
  Node *node;
} State;

State* new_state();

Node* new_int_node(int i);
Node* new_float_node(double d);
Node* new_binop_node(int op, Node *left, Node *right);
Node* new_idt_node(char *idtname);
Node* new_lvar_node(Node *idt);
Node* new_let_node(Node *left, Node *right);
Node* new_init_node(Node *left, Node *right);
Node* new_print_node(Node *target); // Temporary
void append_nodes(Node *nodes, Node *node);
void reverse_nodes(Node *nodes);
Node* new_nodes();
Node* new_funcdef_node(Node *idt, Node *params, Node *block);
Node* new_funccall_node(Node *idt, Node *args);
Node* new_funcparam_node(Node *idt);
Node* new_return_node(Node *retval);

void compile_node(Node *node);
void execute_code();
void print_result();
void debug_code();

#endif
