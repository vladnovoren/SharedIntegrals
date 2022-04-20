#ifndef INTEGRAL_TASK_HPP
#define INTEGRAL_TASK_HPP

#include <functional>

double GetIndexed(double lhs, double rhs, size_t i, size_t n);

struct IntegralTask {
  std::function<double(double)> f = nullptr;

  double lhs_lim_ = 0;
  double rhs_lim_ = 0;

  size_t seg_cnt_ = 0;

  double Complete();
  bool Last();
  
 private:
  double IndexedF(const size_t i);
};


#endif /* integral_task.hpp */

