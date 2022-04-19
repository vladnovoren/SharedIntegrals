#ifndef UNSAFE_SHARED_OBJECT_HPP
#define UNSAFE_SHARED_OBJECT_HPP

#include "shared_memory_object.hpp"

template <typename ObjT>
class UnsafeSharedObject {
public:
  UnsafeSharedObject(find_only_t, const std::string& name) : shared_memory_(find_only, name) {
  }

  template <typename... ArgsT>
  UnsafeSharedObject(create_only_t, const std::string &name, ArgsT &&... args)
      : shared_memory_(create_only, name, sizeof(ObjT)) {
    ObjT* obj_ptr = static_cast<ObjT*>(shared_memory_.Data());
    new (obj_ptr) ObjT(std::forward<ArgsT>(args)...);
  }

  ~UnsafeSharedObject() {
    GetObjPtr()->~ObjT();
  }

  ObjT* GetObjPtr() {
    return static_cast<ObjT*>(shared_memory_.Data());
  }

private:
  SharedMemoryObject shared_memory_;

};

#endif /* unsafe_shared_object.hpp */
