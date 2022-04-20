#include "integral_task.hpp"

double GetIndexed(double lhs, double rhs, size_t i, size_t n) {
  return ((n - i) * lhs + i * rhs) / n;
}

double IntegralTask::Complete() {
  double res = 0;

  double seg_len = (rhs_lim_ - lhs_lim_) / seg_cnt_;
  for (size_t i = 0; i < seg_cnt_; ++i) {
    res += 0.5 * (IndexedF(i) + IndexedF(i + 1)) * seg_len;
  }

  return res;
}

double IntegralTask::IndexedF(const size_t i) {
  return f(GetIndexed(lhs_lim_, rhs_lim_, i, seg_cnt_));
}

bool IntegralTask::Last() {
  return f == nullptr;
}
