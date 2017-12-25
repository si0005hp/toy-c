int sum(int x, int y) {
  return x + y;
}

int inc(int x) {
  return x + 1;
}

int printFunc(int x) {
  print(x);
  return 0;
}

int main() {
  int res;
  res = sum(inc(inc(inc(0))), inc(inc(inc(inc(inc(0))))));

  printFunc(res);
  return 5;
}
