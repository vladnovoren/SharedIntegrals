#include "sem_lock_guard.hpp"

SemLockGuard::SemLockGuard(Semaphore& sem) : sem_(sem) {
  if (sem.GetValue() != 1) {
    throw std::logic_error("unable to guard with non-binary semaphore");
  }
  sem.Wait();
}

SemLockGuard::~SemLockGuard() {
  sem_.Post();
}
