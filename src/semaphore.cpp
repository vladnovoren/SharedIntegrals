#include "semaphore.hpp"

Semaphore::Semaphore(create_only_t, const std::string& name, const unsigned int value)
    : name_(name), creator_pid_(getpid()) {
  sem_ = sem_open(name.c_str(), O_CREAT, S_IRUSR | S_IWUSR, value);
  if (sem_ == SEM_FAILED) {
    std::string error_msg = __PRETTY_FUNCTION__;
    error_msg += '\n';
    error_msg += strerror(errno);
    throw std::runtime_error(error_msg);
  }
}

Semaphore::Semaphore(find_only_t, const std::string& name) : name_(name) {
  sem_ = sem_open(name.c_str(), 0);
  if (sem_ == SEM_FAILED) {
    std::string error_msg = __PRETTY_FUNCTION__;
    error_msg += '\n';
    error_msg += strerror(errno);
    throw std::runtime_error(error_msg);
  }
}

Semaphore::~Semaphore() {
  sem_close(sem_);
  if (IsCreator()) {
    sem_unlink(name_.c_str());
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

bool Semaphore::IsCreator() {
  return getpid() == creator_pid_;
}
