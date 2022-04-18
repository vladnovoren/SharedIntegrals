#include "managed_shared_memory.hpp"

ManagedSharedMemory::ManagedSharedMemory(const std::string& name,
                                         const size_t size)
    : buffer_("buffer_" + name, size), names_addresses_