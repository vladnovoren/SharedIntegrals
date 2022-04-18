#ifndef NAME_ADDRESS_HPP
#define NAME_ADDRESS_HPP

#include <cstddef>

static const size_t MAX_NAME_LEN = 200;

struct NameAddress {
  char name[MAX_NAME_LEN + 1] = {};
  void* obj_ptr = nullptr;

  void* GetObjPtr() {
    return obj_ptr;
  }
};

#endif /* name_address.hpp */
