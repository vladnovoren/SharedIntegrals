#include "shared_memory.hpp"

void ChildRoutine() {
  // sleep(3);
  SharedMemory shared_memory("shared_memory");
  int* ptr = static_cast<int*>(shared_memory.Data());
  *ptr = 1;
}

void ParentRoutine(SharedMemory& shared_memory) {
  int* ptr = static_cast<int*>(shared_memory.Data());
  *ptr = 2;
}

int main() {
  SharedMemory shared_memory("shared_memory", 1024);
  int child_id = fork();
  switch (child_id) {
   case -1:
    perror("bad fork");
    exit(1);
    break;

   case 0:
    // ChildRoutine();
    break;

   default:
    ParentRoutine(shared_memory);
    sleep(5);
    break;
  }
  return 0;
}