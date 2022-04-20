#include "functions.hpp"

double xsin2x(double x) {
  return x * sin(x) * sin(x);
}

double rep(double x) {
  for (size_t i = 0; i < 1000000; ++i) {
    x *= sin(x) * (1 - sin(x));
  }
  return x;
}

double X(double x) {
  return x;
}

double F(double x) {
  return rep(x);
}
