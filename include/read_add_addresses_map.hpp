#ifndef READ_ADD_ADDRESSES_MAP_HPP
#define READ_ADD_ADDRESSES_MAP_HPP

#include "name_address.hpp"
#include <string>
#include <cassert>
#include <stdexcept>
#include <string.h>

class ReadAddAddressesMap {
 public:
  bool TryAdd(const std::string& name, void* address);

  void* Find(const std::string& name);

 private:
  static const size_t MAP_CAP = 4096;

 private:
  bool CheckNameSize(const std::string& name);
  bool CheckCap();
  
 private:
  NameAddress names_[MAP_CAP] = {};
  size_t size_ = 0;

};

#endif /* read_add_addresses_map.hpp */
