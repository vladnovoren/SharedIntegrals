#include "managed_shared_memory.hpp"
#include "unsafe_stack.hpp"
#include <iostream>
#include "blocking_queue.hpp"

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

void TestQueueChildRoutine() {
  ManagedSharedMemory managed_shared_memory(find_only, "for_queue");
  BlockingQueue<int>* queue_ptr = managed_shared_memory.Find<BlockingQueue<int>>("queue");
  Destructor<BlockingQueue<int>> destr(queue_ptr);

  while (true) {
    
  }
}

void TestQueue() {
  ManagedSharedMemory managed_shared_memory(create_only, "for_queue", 1024);
  BlockingQueue<int>* queue_ptr = managed_shared_memory.Construct<BlockingQueue<int>>("queue", 10);
  Destructor<BlockingQueue<int>> destr(queue_ptr);

  Semaphore sem(create_only, "/counter", 0);
  
  pid_t parent_id = getpid();

  MakeChildren(1);

  if (parent_id != getpid()) {
    TestQueueChildRoutine();
    sem.Post();
  } else {

    sem.Wait();
  }
}

int main() {
  // TestChildProcessFind();
  // TestStack();
  TestQueue();
  return 0;
}