#include "managed_shared_memory.hpp"
#include "unsafe_stack.hpp"
#include <iostream>
#include "blocking_queue.hpp"
#include <sys/wait.h>

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

void WaitChildren() {
  while (wait(NULL) > 0) {}
}

void TestQueueChildRoutine() {
  ManagedSharedMemory managed_shared_memory(find_only, "for_queue");
  BlockingQueue<int, 4>* queue_ptr = managed_shared_memory.Find<BlockingQueue<int, 4>>("queue");
  for (size_t i = 0; i < 4; ++i) {
    std::cout << queue_ptr->Take() << '\n';
  }
}

void TestQueue() {
  pid_t parent_id = getpid();

  ManagedSharedMemory managed_shared_memory(create_only, "for_queue", 1024);
  BlockingQueue<int, 4>* queue_ptr = managed_shared_memory.Construct<BlockingQueue<int, 4>>("queue");
  Destructor<BlockingQueue<int, 4>> destr(queue_ptr, parent_id);

  // MakeChildren(1);

  if (parent_id != getpid()) {
    TestQueueChildRoutine();
  } else {
    int x = 0;
    for (size_t i = 0; i < 4; ++i) {
      std::cin >> x;
      queue_ptr->Put(x);
    }
  }

  if (parent_id == getpid()) {
    WaitChildren();
  }
}

int main() {
  TestQueue();
  return 0;
}