#include "semaphore.hpp"

Semaphore::Semaphore(const char *name, const unsigned int value)
    : is_owner_{true}, name_(name) {
  assert(name != nullptr);

  sem_ = sem_open(name, O_CREAT, S_IRUSR | S_IWUSR, value);
  if (sem_ == SEM_FAILED) {
    throw std::runtime_error(strerror(errno));
  }
}

Semaphore::Semaphore(const char *name) : name_(name) {
  assert(name != nullptr);

  sem_ = sem_open(name, 0);
  if (sem_ == SEM_FAILED) {
    throw std::runtime_error(strerror(errno));
  }
}

Semaphore::~Semaphore() {
  if (is_owner_) {
    sem_unlink(name_.c_str());
  } else {
    sem_close(sem_);
  }
}

void Semaphore::Wait() {
  assert(sem_ != nullptr);

  int res = sem_wait(sem_);
  if (res != 0) {
    throw std::runtime_error(strerror(errno));
  }
}

void Semaphore::TryWait() {
  assert(sem_ != nullptr);

  int res = sem_trywait(sem_);
  if (res != 0) {
    throw std::runtime_error(strerror(errno));
  }
}

void Semaphore::Post() {
  assert(sem_ != nullptr);

  int res = sem_post(sem_);
  if (res != 0) {
    throw std::runtime_error(strerror(errno));
  }
}

int Semaphore::GetValue() {
  assert(sem_ != nullptr);

  int value = 0;
  int res = sem_getvalue(sem_, &value);
  if (res != 0) {
    throw std::runtime_error(strerror(errno));
  }
  return value;
}
