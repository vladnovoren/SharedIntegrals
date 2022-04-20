#include "managed_shared_memory.hpp"

ManagedSharedMemory::ManagedSharedMemory(create_only_t,
                                         const std::string& name,
                                         const size_t size)
    : buffer_(create_only, "buffer_" + name, size),
      addresses_map_(create_only, "map_" + name),
      allocator_(create_only, "allocator_" + name, buffer_.Data(), buffer_.Size()),
      locker_(create_only, "/locker_" + name, 1) {
}

ManagedSharedMemory::ManagedSharedMemory(find_only_t,
                                         const std::string& name)
    : buffer_(find_only, "buffer_" + name),
      addresses_map_(find_only, "map_" + name),
      allocator_(find_only, "allocator_" + name),
      locker_(find_only, "/locker_" + name) {
}
