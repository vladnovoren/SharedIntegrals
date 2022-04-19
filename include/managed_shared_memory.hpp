#ifndef MANAGED_SHARED_MEMORY_HPP
#define MANAGED_SHARED_MEMORY_HPP

#include "shared_memory_object.hpp"
#include "unsafe_shared_object.hpp"
#include "linear_allocator.hpp"
#include "read_add_addresses_map.hpp"
#include "sem_lock_guard.hpp"

class ManagedSharedMemory {
 public:
  ManagedSharedMemory(create_only_t, const std::string& name, const size_t size);
  ManagedSharedMemory(find_only_t, const std::string& name);

  template<typename ObjT>
  ObjT* Find(const std::string& name) {
    SemLockGuard lock(sem_);

    return FindUnsafeImpl<ObjT>(name);
  }

  template<typename ObjT, typename... ArgsT>
  ObjT* Construct(const std::string& name, ArgsT&&... args) {
    SemLockGuard lock(sem_);

    return ConstructUnsafeImpl<ObjT>(name, std::forward<ArgsT>(args)...);
  }

 private:
  template<typename ObjT>
  ObjT* FindUnsafeImpl(const std::string& name) {
    assert(addresses_map_.GetObjPtr());
    return static_cast<ObjT*>(addresses_map_.GetObjPtr()->Find(name));
  }

  template<typename ObjT, typename... ArgsT>
  ObjT* ConstructUnsafeImpl(const std::string& name, ArgsT&&... args) {
    if (FindUnsafeImpl<ObjT>(name) != nullptr) {
      throw std::logic_error("object with name [" + name + "] already exists");
      return nullptr;
    }

    void* ptr = allocator_.GetObjPtr()->Allocate(sizeof(ObjT));
    if (ptr == nullptr) {
      return nullptr;
    }
    new (ptr) ObjT(create_only, name, std::forward<ArgsT>(args)...);

    if (!addresses_map_.GetObjPtr()->TryAdd(name, ptr)) {
      return nullptr;
    }

    return static_cast<ObjT*>(ptr);
  }

 private:
  SharedMemoryObject buffer_;

  UnsafeSharedObject<ReadAddAddressesMap> addresses_map_;

  UnsafeSharedObject<LinearAllocator> allocator_;

 private:
  Semaphore sem_;

};

#endif /* managed_shared_memory.hpp */
