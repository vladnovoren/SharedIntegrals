#include "shared_memory.hpp"

SharedMemory::SharedMemory(const char* name) : name_(name) {
  assert(name != nullptr);

  int fd = ShmOpenCreated(name);
  buffer_ = Map(fd, FileSize(fd));
  CloseFD(fd);

  SetPointerFields();
}

SharedMemory::SharedMemory(const char* name, const size_t size) :
  name_(name) {
  assert(name != nullptr);
  
  int fd = ShmCreate(name, size + sizeof(SharedFields));
  Truncate(fd, size + sizeof(SharedFields));
  buffer_ = Map(fd, size + sizeof(SharedFields));
  CloseFD(fd);

  SetPointerFields();
  shared_fields_->size_ = size;
}

SharedMemory::~SharedMemory() {
  shm_unlink(name_.c_str());
}

void* SharedMemory::Data(const size_t offset) {
  return static_cast<uint8_t*>(data_) + offset;
}

size_t SharedMemory::Size() const {
  assert(shared_fields_ != nullptr);

  return shared_fields_->size_;
}

void SharedMemory::SetPointerFields() {
  shared_fields_ = static_cast<SharedFields*>(buffer_);
  data_ = static_cast<uint8_t*>(buffer_) + sizeof(SharedFields);
}

int SharedMemory::ShmCreate(const char* name, const size_t bytes_cnt) {
  assert(name != nullptr);

  int fd = shm_open(name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    throw std::runtime_error(strerror(errno));
  }

  return fd;
}

int SharedMemory::ShmOpenCreated(const char* name) {
  assert(name != nullptr);

  int fd = shm_open(name, O_RDWR, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    printf("balda\n");
    throw std::runtime_error(strerror(errno));
  }

  return fd;
}

void* SharedMemory::Map(const int fd, const size_t bytes_cnt) {
  void* addr = mmap(NULL, bytes_cnt, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (addr == MAP_FAILED) {
    throw std::runtime_error(strerror(errno));
  }
  return addr;
}
