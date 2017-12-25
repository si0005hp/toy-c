int sum(int x, int y, int z) {
  return x + y + z;
}

int inc(int x) {
  return x + 1;
}

int main() {
  int res;
  res = sum(3, 5, 7);
  res = inc(res);
  res = inc(res);
  res = inc(res);

  print(res);
  return 0;
}
