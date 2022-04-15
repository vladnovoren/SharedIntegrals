#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>
#include <cassert>
#include <stdexcept>
#include <string>

class Semaphore {
 public:
  Semaphore(const char* name, const unsigned int value);
  explicit Semaphore(const char* name);
  ~Semaphore();

  void Wait();
  void TryWait();

  void Post();

  int GetValue();

 private:
  sem_t* sem_ = nullptr;
  bool is_owner_ = false;

  const std::string name_;

};

#endif /* semaphore.hpp */
