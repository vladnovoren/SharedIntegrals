#include "shared_memory_linear_allocator.hpp"

SharedMemoryLinearAllocator::SharedMemoryLinearAllocator(const char* name)
    : shared_memory_(name) {
}

SharedMemoryLinearAllocator::SharedMemoryLinearAllocator(const char* name,
                                                         const size_t cap)
    : shared_memory_(name, cap) {
}

