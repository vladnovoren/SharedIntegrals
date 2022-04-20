#include "integral_solver.hpp"

void SingleSolution() {
  double result = 0;

  double lhs_lim = 0;
  double rhs_lim = M_PI;

  IntegralTask task{F, lhs_lim, rhs_lim, FRAC};
  result = task.Complete();

  std::cout << result << '\n';
}

int main() {
  SingleSolution();
  return 0;
}

