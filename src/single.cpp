#include "integral_solver.hpp"

double SingleSolution(std::function<double(double)> f) {
  double result = 0;

  double lhs_lim = 0;
  double rhs_lim = M_PI;

  IntegralTask task{f, lhs_lim, rhs_lim, FRAC};
  result = task.Complete();

  return result;
}

int main() {
  std::cout << SingleSolution(xsin2x) << '\n';
  std::cout << SingleSolution(exsinex) << '\n';
  std::cout << SingleSolution(expx2) << '\n';
  std::cout << SingleSolution(x2sinx2) << '\n';
  std::cout << SingleSolution(x2ln1_plus_x) << '\n';
  std::cout << SingleSolution(x3) << '\n';
  return 0;
}

