#include "fd_helpers.hpp"

struct stat GetFileStat(const int fd) {
  struct stat fd_stat;
  if (fstat(fd, &fd_stat) == -1) {
    throw std::runtime_error(strerror(errno));
  }
  return fd_stat;
}

size_t FileSize(const int fd) {
  struct stat fd_stat;
  if (fstat(fd, &fd_stat) == -1) {
    throw std::runtime_error(strerror(errno));
  }
  return fd_stat.st_size;
}

int Truncate(const int fd, const size_t bytes_cnt) {
  int ret = ftruncate(fd, bytes_cnt);
  if (ret == -1) {
    throw std::runtime_error(strerror(errno));
  }
  return ret;
}

int CloseFD(const int fd) {
  int ret = close(fd);
  if (ret == -1) {
    throw std::runtime_error(strerror(errno));
  }
  return ret;
}
