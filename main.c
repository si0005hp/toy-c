#include <stdio.h>
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
