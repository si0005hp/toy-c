%{
%}
%%
[1-9][0-9]*|0 {
  printf("Number: %s\n", yytext);
}
. {
  printf("NG\n");
}
%%