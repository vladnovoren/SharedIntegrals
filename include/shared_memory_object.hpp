#ifndef SHARED_MEMORY_OBJECT_HPP
#define SHARED_MEMORY_OBJECT_HPP

#include <unistd.h>
#include <fcntl.h>
#include <cassert>
#include <stdio.h>
#include <stdexcept>
#include <string>

#include "fd_helpers.hpp"
#include "semaphore.hpp"

class SharedMemoryObject {
 public:
  SharedMemoryObject(find_only_t, const std::string& name);
  SharedMemoryObject(create_only_t, const std::string& name, const size_t size);
  ~SharedMemoryObject();

  const std::string& Name();

  void* Data(const size_t offset = 0);
  size_t Size();

 private:
  void SetPointerFields();

  int ShmCreate(const std::string& name, const size_t bytes_cnt);
  int ShmOpenCreated(const std::string& name);

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

#endif /* shared_memory_object.hpp */
