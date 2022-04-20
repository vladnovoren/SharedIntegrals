#include "children.hpp"

void MakeChildren(const size_t n) {
  pid_t parent_id = getpid();
  for (size_t i = 0; i < n; ++i) {
    if (getpid() == parent_id) {
      fork();
    }
  }
}

void WaitChildren(const pid_t parent_id) {
  if (parent_id == getpid()) {
    while (wait(NULL) > 0) {}
  }
}
