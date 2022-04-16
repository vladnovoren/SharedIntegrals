#ifndef SHARED_LINEAR_ALLOCATOR_HPP
#define SHARED_LINEAR_ALLOCATOR_HPP

#include "shared_memory_object.hpp"

class SharedMemoryLinearAllocator {
 public:
  SharedMemoryLinearAllocator(const std::string& name, void* buffer, const size_t cap);
  SharedMemoryLinearAllocator(const std::string& name);
  ~SharedMemoryLinearAllocator();

  size_t Cap();

  void* TryAllocate(const size_t size);

 private:
  void* FreeData();

  size_t Used();
  size_t Free();

  bool CanAllocate(const size_t size);

 private:
  SharedMemoryObject shared_memory_;
  Semaphore locker_;

  struct SharedFields {
    size_t cap_;
    size_t used_;
  };

  SharedFields* shared_fields_ = nullptr;

};

#endif /* shared_memory_linear_allocator.hpp */
