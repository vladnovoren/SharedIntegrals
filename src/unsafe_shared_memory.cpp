#include "unsafe_shared_memory.hpp"

UnsafeSharedMemory::UnsafeSharedMemory(const char* name) : name_(name) {
  assert(name != nullptr);

  int fd = ShmOpenCreated(name);
  buffer_ = Map(fd, FileSize(fd));
  CloseFD(fd);

  SetPointerFields();
}

UnsafeSharedMemory::UnsafeSharedMemory(const char* name, const size_t size) :
  name_(name) {
  assert(name != nullptr);
  
  int fd = ShmCreate(name, size + sizeof(SharedFields));
  Truncate(fd, size + sizeof(SharedFields));
  buffer_ = Map(fd, size + sizeof(SharedFields));
  CloseFD(fd);

  SetPointerFields();
  shared_fields_->size_ = size;
}

UnsafeSharedMemory::~UnsafeSharedMemory() {
  shm_unlink(name_.c_str());
}

void* UnsafeSharedMemory::Data(const size_t offset) {
  return static_cast<uint8_t*>(data_) + offset;
}

size_t UnsafeSharedMemory::Size() const {
  assert(shared_fields_ != nullptr);

  return shared_fields_->size_;
}

void UnsafeSharedMemory::SetPointerFields() {
  shared_fields_ = static_cast<SharedFields*>(buffer_);
  data_ = static_cast<uint8_t*>(buffer_) + sizeof(SharedFields);
}

int UnsafeSharedMemory::ShmCreate(const char* name, const size_t bytes_cnt) {
  assert(name != nullptr);

  int fd = shm_open(name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    throw std::runtime_error(strerror(errno));
  }

  return fd;
}

int UnsafeSharedMemory::ShmOpenCreated(const char* name) {
  assert(name != nullptr);

  int fd = shm_open(name, O_RDWR, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    printf("balda\n");
    throw std::runtime_error(strerror(errno));
  }

  return fd;
}

void* UnsafeSharedMemory::Map(const int fd, const size_t bytes_cnt) {
  void* addr = mmap(NULL, bytes_cnt, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (addr == MAP_FAILED) {
    throw std::runtime_error(strerror(errno));
  }
  return addr;
}
