#ifndef UNSAFE_STACK_HPP
#define UNSAFE_STACK_HPP

#include "linear_allocator.hpp"

template <typename ElemT, size_t cap_>
class UnsafeStack {
 public:
  UnsafeStack() {}
  
  ~UnsafeStack() {
    while (!IsEmpty()) {
      Pop();
    }
  }

  template<typename... ArgsT>
  void Push(ArgsT&&... args) {
    if (size_ == cap_) {
      throw std::logic_error(UNABLE_TO_PUSH_);
      return;
    }
    ElemT* allocated = AllocateForPush();
    new (allocated) ElemT(std::forward<ArgsT>(args)...);
    ++size_;
  }

  ElemT& Top() {
    if (size_ == 0) {
      throw std::logic_error(UNABLE_TO_TOP_);
    }
    return *reinterpret_cast<ElemT*>(&buffer_[(size_ - 1) * sizeof(ElemT)]);
  }

  ElemT& Bottom() {
    if (size_ == 0) {
      throw std::logic_error(UNABLE_TO_BOTTOM_);
    }
    return *reinterpret_cast<ElemT*>(buffer_);
  }

  void Pop() {
    if (size_ == 0) {
      throw std::logic_error(UNABLE_TO_POP_);
    }
    (&Top())->~ElemT();
    --size_;
  }

  size_t Size() {
    return size_;
  }

  bool IsEmpty() {
    return size_ == 0;
  }

 private:
  ElemT* AllocateForPush() {
    return reinterpret_cast<ElemT*>(&buffer_[size_ * sizeof(ElemT)]);
  }

 private:
  static const char* const UNABLE_TO_PUSH_;
  static const char* const UNABLE_TO_TOP_;
  static const char* const UNABLE_TO_POP_;
  static const char* const UNABLE_TO_BOTTOM_;

  uint8_t buffer_[cap_ * sizeof(ElemT)];
  size_t size_ = 0;

};

template<typename ElemT, size_t cap_>
const char* const UnsafeStack<ElemT, cap_>::UNABLE_TO_PUSH_ = "unable to push into full stack";

template<typename ElemT, size_t cap_>
const char* const UnsafeStack<ElemT, cap_>::UNABLE_TO_TOP_ = "stack size is zero, unable to get top element";

template<typename ElemT, size_t cap_>
const char* const UnsafeStack<ElemT, cap_>::UNABLE_TO_POP_ = "stack size is zero, unable to pop";

template<typename ElemT, size_t cap_>
const char* const UnsafeStack<ElemT, cap_>::UNABLE_TO_BOTTOM_ = "unable to get element from bottom of empty stack";

#endif /* unsafe_stack.hpp */
