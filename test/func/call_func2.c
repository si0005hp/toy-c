int foo() {
  return 1;
}

int bar() {
  return 3;
}

int baz() {
  return 5;
}

int main() {
  int x = foo();
  int y = bar();
  int z = baz();
  int sum = x + y + z;
  print(sum);

  x = baz();
  y = bar();
  z = foo();
  sum = x + y + z;
  print(sum);

  sum = foo() + bar() + baz();
  print(sum);

  return 0;
}
