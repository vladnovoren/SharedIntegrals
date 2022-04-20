#include "sem_lock_guard.hpp"

SemLockGuard::SemLockGuard(Semaphore& sem) : sem_(sem) {
  sem_.Wait();
}

SemLockGuard::~SemLockGuard() {
  sem_.Post();
}
