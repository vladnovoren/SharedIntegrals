#include "shm_linear_allocator.hpp"
#include <iostream>
#include <string>

int main(int argc, const char*[]) {
  try {
    if (argc == 1) { // master
      SharedMemory allocator("MyAllocator", 1024);
      int* arr = (int*)allocator.Reserve(sizeof(int) * 10);
      for (size_t i = 0; i < 10; ++i) {
        arr[i] = i;
      }

      std::string str;
      getline(std::cin, str);

      for (size_t i = 0; i < 10; ++i) {
        std::cout << arr[i] << ' ';
      }
      std::cout << '\n';
    } else {
      SharedMemory allocator("MyAllocator");

      int* arr = (int*)allocator.Reserve(sizeof(int) * 10);
      std::cout << allocator.FreeBytesCnt() << '\n';

      for (size_t i = 0; i < 10; ++i) {
        std::cout << arr[i] << ' ';
      }
      std::cout << '\n';

      for (size_t i = 0; i < 10; ++i) {
        arr[i] = 9 - i;
      }
    }
  } catch (std::exception& e) {
    std::cout << e.what() << '\n';
  }
  return 0;
}
