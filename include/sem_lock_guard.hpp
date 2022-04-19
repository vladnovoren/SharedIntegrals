#ifndef SEM_LOCK_GUARD_HPP
#define SEM_LOCK_GUARD_HPP

#include "semaphore.hpp"

class SemLockGuard {
 public:
  SemLockGuard(Semaphore& sem);
  ~SemLockGuard();

 private:
  Semaphore& sem_;

};

#endif /* sem_lock_guard.hpp */
