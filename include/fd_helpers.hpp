#ifndef FD_HELPERS_HPP
#define FD_HELPERS_HPP

#include <sys/mman.h>
#include <sys/stat.h>
#include <stdexcept>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

struct stat GetFileStat(const int fd);

size_t FileSize(const int fd);

int Truncate(const int fd, const size_t bytes_cnt);

int CloseFD(const int fd);

#endif /* fd_helpers.hpp */
