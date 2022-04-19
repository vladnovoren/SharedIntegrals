#ifndef BLOCKING_QUEUE_HPP
#define BLOCKING_QUEUE_HPP

#include "unsafe_stack.hpp"
#include "semaphore.hpp"
#include "sem_lock_guard.hpp"
#include <utility>

template<typename ElemT>
class BlockingQueue {
 public:
  BlockingQueue(find_only_t, const std::string& name)
      : locker_(find_only, "/queue_locker_" + name) {
  }

  BlockingQueue(create_only_t, const std::string& name, const size_t cap)
      : cap_(cap),
        free_(cap),
        used_(0),
        locker_(create_only, "/queue_locker_" + name, 1) {
    SemLockGuard lock(locker_);

    push_stack_ = reinterpret_cast<UnsafeStack<ElemT>*>(this + 1);
    new (push_stack_) UnsafeStack<ElemT>(cap);
    pop_stack_ = reinterpret_cast<UnsafeStack<ElemT>*>(reinterpret_cast<uint8_t*>(push_stack_) + UnsafeStack<ElemT>::CalcSize(cap));
    new (pop_stack_) UnsafeStack<ElemT>(cap);
  }

  ~BlockingQueue() {}

  template<typename... ArgsT>
  void Put(ArgsT&&... args) {
    free_.Wait();
  
    SemLockGuard lock(locker_);

    if (size_ == cap_) {
      throw std::logic_error(UNABLE_TO_PUSH_);
    }
    push_stack_->TryPush(std::forward<ArgsT>(args)...);
    ++size_;

    used_.Post();
  }

  ElemT Take() {
    used_.Wait();

    SemLockGuard lock(locker_);

    ValidStacks();
    
    pop_stack_->Pop();

    free_.Post();
  }

 private:
  void ValidStacks() {
    if (pop_stack_->IsEmpty()) {
      while (!push_stack_->IsEmpty()) {
        pop_stack_->TryPush(std::move(push_stack_->Top()));
        push_stack_->Pop();
      }
    }
  }

 private:
  static const char* const UNABLE_TO_PUSH_;
  static const char* const UNABLE_TO_POP_;
  static const char* const UNABLE_TO_BACK_;
  static const char* const UNABLE_TO_FRONT_;

 private:
  UnsafeStack<ElemT>* push_stack_ = nullptr;
  UnsafeStack<ElemT>* pop_stack_ = nullptr;

 private:
  size_t size_ = 0;
  size_t cap_ = 0;

 private:
  Semaphore free_;
  Semaphore used_;
  Semaphore locker_;

};

template<typename ElemT>
const char* const BlockingQueue<ElemT>::UNABLE_TO_PUSH_ = "unable to push in full queue";

template<typename ElemT>
const char* const BlockingQueue<ElemT>::UNABLE_TO_POP_ = "unable to pop from empty queue";

template<typename ElemT>
const char* const BlockingQueue<ElemT>::UNABLE_TO_BACK_ = "unable to get back element of empty queue";

template<typename ElemT>
const char* const BlockingQueue<ElemT>::UNABLE_TO_FRONT_ = "unable to get front element of empty queue";

#endif /* blocking_queue.hpp */
