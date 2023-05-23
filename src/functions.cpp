#include "functions.hpp"

double xsin2x(double x) {
  return x * sin(x) * sin(x);
}

double exsinex(double x) {
  return exp(x) * sin(exp(x));
}

double expx2(double x) {
  return std::exp(x * x);
}

double x2sinx2(double x) {
  return x * x * std::sin(x * x);
}

double x2ln1_plus_x(double x) {
  return x * x * std::log(1 + x);
}

double x3(double x) {
  return x * x * x;
}

