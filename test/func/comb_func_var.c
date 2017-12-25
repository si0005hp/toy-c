int unit_price() {
  return 250;
}

int tax(int price) {
  return price * 1.05;
}

int total(int num) {
  int total = num * unit_price();
  return tax(total);
}

int main() {
  int num = 12;
  int res = total(num);
  print(res);
  return 0;
}
