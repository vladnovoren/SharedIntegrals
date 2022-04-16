#ifndef SHARED_MEMORY_QUEUE_HPP
#define SHARED_MEMORY_QUEUE_HPP

#include "shared_memory_linear_allocator.hpp"

class SharedMemoryBoundedQueue {
 public:
  SharedMemoryBoundedQueue(const std::string& name);
  ~SharedMemoryBoundedQueue();
  
 private:
  

};

#endif /* shared_memory_queue.hpp */
