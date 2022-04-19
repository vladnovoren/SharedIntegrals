#include "managed_shared_memory.hpp"
#include "unsafe_stack.hpp"
#include <iostream>

struct Point {
  Point() {}
  Point(int x, int y) : x_(x), y_(y) {}

  int x_ = 0;
  int y_ = 0;
};

void MakeChildren(const size_t n) {
  pid_t parent_id = getpid();
  for (size_t i = 0; i < n; ++i) {
    if (getpid() == parent_id) {
      fork();
    }
  }
}

void TestOneProcess() {
  ManagedSharedMemory managed_shared_memory(create_only, "shared_memory", 1024);
  Point* point_ptr = managed_shared_memory.Construct<Point>("point", 1, 2);
  std::cout << point_ptr->x_ << ' ' << point_ptr->y_ << '\n';
}

void TestChildProcesses() {
  ManagedSharedMemory managed_shared_memory(create_only, "shared_memory", 1024);
  Point* point_ptr = managed_shared_memory.Construct<Point>("point", 1, 2);

  MakeChildren(4);

  assert(point_ptr->x_ == 1 && point_ptr->y_ == 2);
}

void TestChildProcessFindChildRoutine() {
  ManagedSharedMemory managed_shared_memory(find_only, "shared_memory");
  Point* point_ptr = managed_shared_memory.Find<Point>("point");
  assert(point_ptr->x_ == 1 && point_ptr->y_ == 2);
}

void TestChildProcessFind() {
  ManagedSharedMemory managed_shared_memory(create_only, "shared_memory", 1024);
  Point* point_ptr = managed_shared_memory.Construct<Point>("point", 1, 2);

  assert(point_ptr->x_ == 1 && point_ptr->y_ == 2);

  pid_t parend_id = getpid();
  Semaphore counter(create_only, "counter", 4);

  MakeChildren(4);

  if (getpid() != parend_id) {
    TestChildProcessFindChildRoutine();
    counter.Wait();
  } else {
    while (counter.GetValue() != 0) {
    }
  }
}

void TestStackChildRoutine() {
  ManagedSharedMemory managed_shared_memory(find_only, "for_stack");
  UnsafeStack<int>* stack_ptr = managed_shared_memory.Find<UnsafeStack<int>>("stack");

  for (size_t i = 0; i < 10; ++i) {
    stack_ptr->TryPush(i);
  }

  for (size_t i = 0; i < 10; ++i) {
    int x = stack_ptr->Top();
    std::cout << x << ' ';
    stack_ptr->Pop();
  }
  std::cout << '\n';
  std::cout << std::boolalpha << stack_ptr->IsEmpty() << '\n';
}

void TestStack() {
  ManagedSharedMemory managed_shared_memory(create_only, "for_stack", 1024);
  /*UnsafeStack<int>* stack_ptr = */managed_shared_memory.Construct<UnsafeStack<int>>("stack", 10);

  pid_t parend_id = getpid();
  Semaphore counter(create_only, "counter", 1);

  MakeChildren(1);

  if (getpid() != parend_id) {
    TestStackChildRoutine();
    counter.Wait();
  } else {
    while (counter.GetValue() != 0) {
    }
  }
}

int main() {
  // TestChildProcessFind();
  TestStack();
  return 0;
}