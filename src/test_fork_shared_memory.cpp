#include "shared_memory_linear_allocator.hpp"

void ChildRoutine() {
  SharedMemoryObject shared_memory("shared_memory");
  int* ptr = static_cast<int*>(shared_memory.Data());
  ptr[0]++;
}

void ChildRoutineReference(SharedMemoryObject& shared_memory) {
  int* ptr = static_cast<int*>(shared_memory.Data());
  ptr[0]++;
}

void ParentRoutine(SharedMemoryObject& shared_memory) {
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

void ChildRoutineAllocator(SharedMemoryLinearAllocator& allocator, size_t num) {
  int* ptr = static_cast<int*>(allocator.TryAllocate(sizeof(int)));
  assert(ptr != nullptr);
  Semaphore locker("locker", 1);
  Semaphore counter("counter", 4);
  counter.Post();
  if (counter.GetValue() == 1) {
    locker.Post();
  }
  ptr[0] = num;
  locker.Wait();
  printf("ptr: %p, num: %d\n", ptr, ptr[0]);
  locker.Post();
}

void TestSharedMemoryLinearAllocator() {
  SharedMemoryLinearAllocator allocator("allocator", 1024);
  pid_t parend_id = getpid();

  Semaphore children_counter("children_counter", 4);

  MakeChildren(4);

  if (parend_id != getpid()) {
    ChildRoutineAllocator(allocator, children_counter.GetValue());
    children_counter.Post();
  } else {
    while (children_counter.GetValue() != 4) {
    }
  }
}

void ChildrenAfterParentTest() {
  SharedMemoryObject shared_memory("shared_memory", 1024);

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
  // ChildrenAfterParentTest();
  TestSharedMemoryLinearAllocator();
  return 0;
}
