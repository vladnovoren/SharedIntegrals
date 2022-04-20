#include "functions.hpp"

double xsin2x(double x) {
  return x * sin(x) * sin(x);
}

double rep(double x) {
  for (size_t i = 0; i < 1000000; ++i) { // some heavy computing
    x *= sin(x) * (1 - sin(x));
  }
  return x;
}

double X(double x) {
  return x;
}

double F(double x) {
  // return xsin2x(x);
  // return x;
  // return cos(x);
  // return sin(x);
  return rep(x); // here you can call your own function
}
