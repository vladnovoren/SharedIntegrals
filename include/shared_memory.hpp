#ifndef SHARED_MEMORY_HPP
#define SHARED_MEMORY_HPP

#include <unistd.h>
#include <fcntl.h>
#include <cassert>
#include <stdio.h>
#include <stdexcept>
#include <string>

#include "fd_helpers.hpp"
#include "semaphore.hpp"

class SharedMemory {
 public:
  SharedMemory(const char* name);
  SharedMemory(const char* name, const size_t size);
  ~SharedMemory();

  const std::string& Name();

  void* Data(const size_t offset = 0);
  size_t Size() const;

 private:
  void SetPointerFields();

  int ShmCreate(const char* name, const size_t bytes_cnt);
  int ShmOpenCreated(const char* name);

  void* Map(const int fd, const size_t bytes_cnt);

  bool IsCreator();

 private:
  struct SharedFields {
    size_t size_;
  };

  void* buffer_ = nullptr;
  void* data_ = nullptr;
  SharedFields* shared_fields_ = nullptr;

  std::string memory_name_;

  pid_t creator_pid_ = -1;

};

#endif /* shared_memory.hpp */
