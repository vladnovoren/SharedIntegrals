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
  void Deallocate(const size_t size);

  void* Data(const size_t offset = 0);

 private:
  static const char* const UNABLE_TO_ALLOCATE_MSG_;
  static const char* const UNABLE_TO_DEALLOCATE_MSG_;

 private:
  void* buffer_ = nullptr;
  size_t used_ = 0;
  size_t cap_ = 0;

};

#endif /* linear_allocator.cpp */
