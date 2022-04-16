#include "shared_memory_object.hpp"

SharedMemoryObject::SharedMemoryObject(const std::string& name)
    : memory_name_(name) {
  int fd = ShmOpenCreated(name);
  buffer_ = Map(fd, FileSize(fd));
  CloseFD(fd);

  SetPointerFields();
}

SharedMemoryObject::SharedMemoryObject(const std::string& name, const size_t size)
    : memory_name_(name), creator_pid_(getpid()) {
  int fd = ShmCreate(name, size + sizeof(SharedFields));
  Truncate(fd, size + sizeof(SharedFields));
  buffer_ = Map(fd, size + sizeof(SharedFields));
  CloseFD(fd);

  SetPointerFields();
  shared_fields_->size_ = size;
}

SharedMemoryObject::~SharedMemoryObject() {
  if (IsCreator()) {
    shm_unlink(memory_name_.c_str());
  }
}

void *SharedMemoryObject::Data(const size_t offset) {
  return static_cast<uint8_t *>(data_) + offset;
}

size_t SharedMemoryObject::Size() {
  assert(shared_fields_ != nullptr);

  return shared_fields_->size_;
}

void SharedMemoryObject::SetPointerFields() {
  shared_fields_ = static_cast<SharedFields *>(buffer_);
  data_ = static_cast<uint8_t *>(buffer_) + sizeof(SharedFields);
}

int SharedMemoryObject::ShmCreate(const std::string& name, const size_t bytes_cnt) {
  int fd = shm_open(name.c_str(), O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    std::string error_msg = __PRETTY_FUNCTION__;
    error_msg += '\n';
    error_msg += strerror(errno);
    throw std::runtime_error(error_msg);
  }

  return fd;
}

int SharedMemoryObject::ShmOpenCreated(const std::string& name) {
  int fd = shm_open(name.c_str(), O_RDWR, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    std::string error_msg = __PRETTY_FUNCTION__;
    error_msg += '\n';
    error_msg += strerror(errno);
    throw std::runtime_error(error_msg);
  }

  return fd;
}

void *SharedMemoryObject::Map(const int fd, const size_t bytes_cnt) {
  void *addr = mmap(NULL, bytes_cnt, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (addr == MAP_FAILED) {
    std::string error_msg = __PRETTY_FUNCTION__;
    error_msg += '\n';
    error_msg += strerror(errno);
    throw std::runtime_error(error_msg);
  }
  return addr;
}

bool SharedMemoryObject::IsCreator() {
  return getpid() == creator_pid_;
}
