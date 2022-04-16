#ifndef MANAGED_SHARED_MEMORY_HPP
#define MANAGED_SHARED_MEMORY_HPP

#include "shared_memory_object.hpp"

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
  SharedMemoryObject allocator_;

};

#endif /* managed_shared_memory.hpp */
