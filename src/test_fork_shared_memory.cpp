#include "shared_memory.hpp"

void ChildRoutine() {
  SharedMemory shared_memory("shared_memory");
  int* ptr = static_cast<int*>(shared_memory.Data());
  ptr[0]++;
}

void ChildRoutineReference(SharedMemory& shared_memory) {
  int* ptr = static_cast<int*>(shared_memory.Data());
  ptr[0]++;
}

void ParentRoutine(SharedMemory& shared_memory) {
  int* ptr = static_cast<int*>(shared_memory.Data());
  ptr[0] = 0;
}

void MakeChildren(const size_t n) {
  pid_t parent_id = getpid();
  for (size_t i = 0; i < n; ++i) {
    if (getpid() == parent_id) {
      fork();
    }
  }
}

void ChildrenAfterParentTest() {
  SharedMemory shared_memory("shared_memory", 1024);

  printf("parend_id: %d\n", getpid());

  Semaphore parent_finished("parent_finished", 1);
  Semaphore child_counter("child_counter", 4);

  Semaphore child_locker("child_locker", 1);
  child_locker.Post();

  pid_t parent_id = getpid();

  MakeChildren(4);

  switch (getpid() == parent_id) {
   case 1:
    ParentRoutine(shared_memory);
    parent_finished.Post();
    while (child_counter.GetValue() != 4) {}
    break;

   default:
    parent_finished.Wait();
    child_locker.Wait();
    ChildRoutineReference(shared_memory);
    child_locker.Post();
    parent_finished.Post();
    child_counter.Post();
    break;
  }

  int* ptr = static_cast<int*>(shared_memory.Data());
  if (getpid() == parent_id) {
    printf("%d\n", *ptr);
  }
}

int main() {
  ChildrenAfterParentTest();
  return 0;
}
