#include "linear_allocator.hpp"

static const char* const UNABLE_TO_ALLOCATE_MSG_ = "unable to allocate memory";

LinearAllocator::LinearAllocator(void* buffer, const size_t cap)
    : buffer_(buffer), cap_(cap) {}

LinearAllocator::~LinearAllocator() {}

size_t LinearAllocator::Cap() {
  return cap_;
}

size_t LinearAllocator::Used() {
  return used_;
}

size_t LinearAllocator::Free() {
  return Cap() - Used();
}

bool LinearAllocator::CanAllocate(const size_t size) {
  return size <= Free();
}

void* LinearAllocator::Allocate(const size_t size) {
  if (!CanAllocate(size)) {
    throw std::out_of_range(UNABLE_TO_ALLOCATE_MSG_);
    return nullptr;
  }
  used_ += size;
  return Data(used_);
}

void* LinearAllocator::Data(const size_t offset) {
  return static_cast<uint8_t*>(buffer_) + offset;
}

