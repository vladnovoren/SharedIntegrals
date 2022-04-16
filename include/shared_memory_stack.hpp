#ifndef SHARED_MEMORY_BOUNDED_STACK_HPP
#define SHARED_MEMORY_BOUNDED_STACK_HPP

#include "shared_memory_linear_allocator.hpp"

template <typename ElemT> class SharedMemoryBoundedStack {
public:
  SharedMemoryBoundedStack(const std::string &name, const size_t cap)
      : allocator_("stack_allocator" + name, sizeof(SharedData) + sizeof(ElemT) * cap),
        semaphore_("/sem_stack" + name, 1),
        shared_data_(static_cast<SharedData*>(allocator_.TryAllocate(sizeof(SharedData)))) {
    shared_data_->cap_ = cap;
    shared_data_->size_ = 0;
    semaphore_.Post();
  }

  SharedMemoryBoundedStack(const std::string &name)
      : allocator_(name),
        semaphore_("/sem_stack" + name),
        shared_data_(static_cast< {
    
  }

  ~SharedMemoryBoundedStack() {}

private:
  SharedMemoryLinearAllocator allocator_;
  Semaphore semaphore_;

  struct SharedData {
    size_t size_ = 0;
    size_t cap_ = 0;
  };

  SharedData* shared_data_ = nullptr;

};

#endif
