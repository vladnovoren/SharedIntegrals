#include "shared_memory_linear_allocator.hpp"

SharedMemoryLinearAllocator::SharedMemoryLinearAllocator(const std::string &name, const size_t cap)
    : shared_memory_(name, cap + sizeof(SharedFields)),
      locker_("sem_locker_" + name, 1),
      shared_fields_{static_cast<SharedFields *>(shared_memory_.Data())} {
  shared_fields_->cap_ = cap;
  shared_fields_->used_ = 0;
  locker_.Post();
}

SharedMemoryLinearAllocator::SharedMemoryLinearAllocator(const std::string &name)
    : shared_memory_(name), locker_("sem_locker_" + name),
      shared_fields_{static_cast<SharedFields*>(shared_memory_.Data())} {
}

SharedMemoryLinearAllocator::~SharedMemoryLinearAllocator() {}

void* SharedMemoryLinearAllocator::FreeData() {
  return static_cast<uint8_t*>(shared_memory_.Data()) +
         sizeof(SharedFields) + shared_fields_->used_;
}

size_t SharedMemoryLinearAllocator::Cap() {
  assert(shared_fields_ != nullptr);

  return shared_fields_->cap_;
}

size_t SharedMemoryLinearAllocator::Used() {
  assert(shared_fields_ != nullptr);

  return shared_fields_->used_;
}

size_t SharedMemoryLinearAllocator::Free() {
  assert(shared_fields_ != nullptr);

  return Cap() - Used();
}

bool SharedMemoryLinearAllocator::CanAllocate(const size_t size) {
  return size <= Free();
}

void* SharedMemoryLinearAllocator::TryAllocate(const size_t size) {
  locker_.Wait();
  if (!CanAllocate(size)) {
    locker_.Post();
    return nullptr;
  } else {
    shared_fields_->used_ += size;
    locker_.Post();
    return FreeData();
  }
}
