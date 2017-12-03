%{
  #include <stdio.h>
%}
%union {
  int value;
}
%token <value> INTEGER
%type <value> factor
%%
factor: INTEGER { printf("Value: %d\n", $1); };
%%
int yyerror(char const *str) {
  fprintf(stderr, "%s\n", str);
  return 0;
}
int main(void) {
  yyparse();
  return 0;
}