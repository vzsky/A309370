#include <utils/MoreMath.h>

int pow(int base, int k) {
  int result = 1;
  while (k > 0) {
    if (k & 1)
      result = (result * base);
    base = (base * base);
    k >>= 1;
  }
  return result;
}

int pow(int base, int k, int mod) {
  int result = 1;
  int b = base % mod;
  while (k > 0) {
    if (k & 1)
      result = (result * b) % mod;
    b = (b * b) % mod;
    k >>= 1;
  }
  return result;
}

int gcd(int a, int b) {
  while (b != 0) {
    int r = a % b;
    a = b;
    b = r;
  }
  return a;
}

int lcm(int a, int b) { return a * b / gcd(a, b); }
