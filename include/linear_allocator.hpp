#ifndef LINEAR_ALLOCATOR_HPP
#define LINEAR_ALLOCATOR_HPP

#include <stddef.h>
#include <stdexcept>

class LinearAllocator {
 public:
  LinearAllocator(void* buffer, const size_t cap);
  ~LinearAllocator();

  size_t Cap();
  size_t Used();
  size_t Free();

  bool CanAllocate(const size_t size);

  void* Allocate(const size_t size);

 private:
  static const char* const UNABLE_TO_ALLOCATE_MSG_;

 private:
  void* Data(const size_t offset);

 private:
  void* buffer_ = nullptr;
  size_t used_ = 0;
  size_t cap_ = 0;

};

#endif /* linear_allocator.cpp */
