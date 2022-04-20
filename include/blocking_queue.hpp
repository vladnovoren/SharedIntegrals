#ifndef BLOCKING_QUEUE_HPP
#define BLOCKING_QUEUE_HPP

#include "unsafe_stack.hpp"
#include "semaphore.hpp"
#include "sem_lock_guard.hpp"
#include <utility>

template<typename ElemT, size_t cap_>
class BlockingQueue {
 public:
  BlockingQueue(find_only_t, const std::string& name)
      : free_(find_only, "/free_counter_" + name),
        used_(find_only, "/used_counter_" + name),
        locker_(find_only, "/queue_locker_" + name) {
  }

  BlockingQueue(create_only_t, const std::string& name)
      : free_(create_only, "/free_counter_" + name, cap_),
        used_(create_only, "/used_counter_" + name, 0),
        locker_(create_only, "/queue_locker_" + name, 1) {
  }

  ~BlockingQueue() {
  }

  template<typename... ArgsT>
  void Put(ArgsT&&... args) {
    free_.Wait();

    SemLockGuard lock(locker_);

    push_stack_.Push(std::forward<ArgsT>(args)...);

    used_.Post();
  }

  ElemT Take() {
    used_.Wait();

    SemLockGuard lock(locker_);

    ValidBeforePop();
    ElemT front(std::move(pop_stack_.Top()));
    pop_stack_.Pop();

    free_.Post();

    return front;
  }

 private:
  void ValidBeforePop() {
    if (pop_stack_.IsEmpty()) {
      while (!push_stack_.IsEmpty()) {
        pop_stack_.Push(std::move(push_stack_.Top()));
        push_stack_.Pop();
      }
    }
  }

 private:
  static const char* const UNABLE_TO_PUSH_;
  static const char* const UNABLE_TO_POP_;
  static const char* const UNABLE_TO_BACK_;
  static const char* const UNABLE_TO_FRONT_;

 private:
  UnsafeStack<ElemT, cap_> push_stack_;
  UnsafeStack<ElemT, cap_> pop_stack_;

 private:
  size_t size_ = 0;

 private:
  Semaphore free_;
  Semaphore used_;
  Semaphore locker_;

};

template<typename ElemT, size_t cap_>
const char* const BlockingQueue<ElemT, cap_>::UNABLE_TO_PUSH_ = "unable to push in full queue";

template<typename ElemT, size_t cap_>
const char* const BlockingQueue<ElemT, cap_>::UNABLE_TO_POP_ = "unable to pop from empty queue";

template<typename ElemT, size_t cap_>
const char* const BlockingQueue<ElemT, cap_>::UNABLE_TO_BACK_ = "unable to get back element of empty queue";

template<typename ElemT, size_t cap_>
const char* const BlockingQueue<ElemT, cap_>::UNABLE_TO_FRONT_ = "unable to get front element of empty queue";

#endif /* blocking_queue.hpp */
