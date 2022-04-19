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
#include "help_types.hpp"

class Semaphore {
 public:
  Semaphore(create_only_t, const std::string& name, const unsigned int value);
  explicit Semaphore(find_only_t, const std::string& name);
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
