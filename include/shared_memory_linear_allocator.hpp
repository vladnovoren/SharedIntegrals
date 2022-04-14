#ifndef SHARED_MEMORY_LINEAR_ALLOCATOR_HPP
#define SHARED_MEMORY_LINEAR_ALLOCATOR_HPP

#include "shared_memory.hpp"

class SharedMemoryLinearAllocator {
 public:
  SharedMemoryLinearAllocator(const char* name);
  SharedMemoryLinearAllocator(const char* name, const size_t cap);
  ~SharedMemoryLinearAllocator();

  void* Reserve(const size_t size);
  size_t Capacity();
  size_t Used();
  size_t Remained();

 private:
  SharedMemory shared_memory_;

};

#endif /* shared_memory_linear_allocator.hpp */
