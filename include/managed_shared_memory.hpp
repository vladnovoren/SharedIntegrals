#ifndef MANAGED_SHARED_MEMORY_HPP
#define MANAGED_SHARED_MEMORY_HPP

#include "shared_memory_object.hpp"
#include "unsafe_shared_object.hpp"
#include "linear_allocator.hpp"

class ManagedSharedMemory {
 public:
  ManagedSharedMemory(const std::string& name, const size_t size);
  ManagedSharedMemory(const std::string& name);

  template<typename ObjT>
  ObjT* Find(const std::string& name) {

  }

  template<typename ObjT, typename... ArgsT>
  ObjT* Construct(ArgsT&&... args) {

  }

 private:
  SharedMemoryObject buffer_;
  SharedMemoryObject names_addresses_;

  UnsafeSharedObject<LinearAllocator> allocator_;

};

#endif /* managed_shared_memory.hpp */
