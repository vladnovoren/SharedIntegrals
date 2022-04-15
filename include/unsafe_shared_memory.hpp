#ifndef UNSAFE_SHARED_MEMORY_HPP
#define UNSAFE_SHARED_MEMORY_HPP

#include <unistd.h>
#include <fcntl.h>
#include <cassert>
#include <stdio.h>
#include <stdexcept>
#include <string>

#include "fd_helpers.hpp"

class UnsafeSharedMemory {
 public:
  explicit UnsafeSharedMemory(const char* name);
  UnsafeSharedMemory(const char* name, const size_t size);
  virtual ~UnsafeSharedMemory() = 0;

  const std::string& Name();

  void* Data(const size_t offset = 0);
  size_t Size() const;

 private:
  void SetPointerFields();

  int ShmCreate(const char* name, const size_t bytes_cnt);
  int ShmOpenCreated(const char* name);

  void* Map(const int fd, const size_t bytes_cnt);

 protected:
  struct SharedFields {
    size_t size_;
  };

  void* buffer_ = nullptr;
  void* data_ = nullptr;
  SharedFields* shared_fields_ = nullptr;

  std::string name_;

};

#endif /* unsafe_shared_memory.hpp */
