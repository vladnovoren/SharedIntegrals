#include "read_add_addresses_map.hpp"

bool ReadAddAddressesMap::TryAdd(const std::string& name, void* address) {
  assert(address != nullptr);

  if (!CheckNameSize(name)) {
    return false;
  }

  if (!CheckCap()) {
    return false;
  }

  if (!memcpy(names_[size_].name, name.c_str(), name.size())) {
    throw std::runtime_error("memcpy error");
    return false;
  }
  names_[size_].obj_ptr = address;
}

void* ReadAddAddressesMap::Find(const std::string& name) {
  if (CheckNameSize(name)) {
    return nullptr;
  }

  for (size_t i = 0; i < size_; ++i) {
    if (name == names_[i].name) {
      return names_[i].obj_ptr;
    }
  }

  return nullptr;
}

bool ReadAddAddressesMap::CheckNameSize(const std::string& name) {
  if (name.size() > MAX_NAME_LEN) {
    throw std::logic_error("too long object name");
    return false;
  }
  return true;
}

bool ReadAddAddressesMap::CheckCap() {
  if (size_ == MAP_CAP) {
    return false;
  }
  return true;
}
