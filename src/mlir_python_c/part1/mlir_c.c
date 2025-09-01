int loop_add(int lb, int ub, int step) {
  int sum_0 = 0;
  int sum = sum_0;
  for (int iv = lb; iv < ub; iv += step) {
    int sum_next = sum + iv;
    sum = sum_next;
  }
  return sum;
}

int main() {
  int out = loop_add(0, 10, 1);
  return out;
}