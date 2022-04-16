#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
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

  bool IsCreator();

 private:
  sem_t* sem_ = nullptr;

  const std::string name_;

  pid_t creator_pid_ = -1;

};

#endif /* semaphore.hpp */
