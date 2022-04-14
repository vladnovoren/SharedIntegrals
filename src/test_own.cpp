#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cassert>
#include <iostream>

int main(int argc, const char* argv[]) {
  if (argc == 1) {
    size_t size = 1024;

    int fd = shm_open("my_shared", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
      perror("shm_open error");
    }
    ftruncate(fd, size);
    void* addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
      perror("mmap error");
    }

    if (close(fd) == -1) {
      perror("close fd error");
    }
    int* arr = static_cast<int*>(addr);
    for (int i = 0; i < 100; ++i) {
      arr[i] = i;
    }
    char c;
    std::cin >> c;
  } else {
    struct stat sb;
    int fd = shm_open("my_shared", O_RDONLY, 0);
    void* addr = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    close(fd);
    int* arr = static_cast<int*>(addr);
    for (int i = 0; i < 100; ++i) {
      std::cout << arr[i] << ' ';
    }
    std::cout << '\n';
  }
  return 0;
}