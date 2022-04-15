#include "semaphore.hpp"

Semaphore::Semaphore(const char *name, const unsigned int value)
    : is_owner_{true}, name_(name) {
  assert(name != nullptr);

  sem_ = sem_open(name, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);
  if (sem_ == SEM_FAILED) {
    std::string error_msg = __PRETTY_FUNCTION__;
    error_msg += '\n';
    error_msg += strerror(errno);
    throw std::runtime_error(error_msg);
  }
}

Semaphore::Semaphore(const char *name) : name_(name) {
  assert(name != nullptr);

  sem_ = sem_open(name, 0);
  if (sem_ == SEM_FAILED) {
    std::string error_msg = __PRETTY_FUNCTION__;
    error_msg += '\n';
    error_msg += strerror(errno);
    throw std::runtime_error(error_msg);
  }
}

Semaphore::~Semaphore() {
  if (is_owner_) {
    printf("unlink\n");
    sem_unlink(name_.c_str());
  } else {
    printf("close\n");
    sem_close(sem_);
  }
}

void Semaphore::Wait() {
  assert(sem_ != nullptr);

  int res = sem_wait(sem_);
  if (res != 0) {
    std::string error_msg = __PRETTY_FUNCTION__;
    error_msg += '\n';
    error_msg += strerror(errno);
    throw std::runtime_error(error_msg);
  }
}

void Semaphore::TryWait() {
  assert(sem_ != nullptr);

  int res = sem_trywait(sem_);
  if (res != 0) {
    std::string error_msg = __PRETTY_FUNCTION__;
    error_msg += '\n';
    error_msg += strerror(errno);
    throw std::runtime_error(error_msg);
  }
}

void Semaphore::Post() {
  assert(sem_ != nullptr);

  int res = sem_post(sem_);
  if (res != 0) {
    std::string error_msg = __PRETTY_FUNCTION__;
    error_msg += '\n';
    error_msg += strerror(errno);
    throw std::runtime_error(error_msg);
  }
}

int Semaphore::GetValue() {
  assert(sem_ != nullptr);

  int value = 0;
  int res = sem_getvalue(sem_, &value);
  if (res != 0) {
    std::string error_msg = __PRETTY_FUNCTION__;
    error_msg += '\n';
    error_msg += strerror(errno);
    throw std::runtime_error(error_msg);
  }
  return value;
}
