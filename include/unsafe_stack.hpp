#ifndef UNSAFE_STACK_HPP
#define UNSAFE_STACK_HPP

#include "linear_allocator.hpp"

template <typename ElemT>
class UnsafeStack {
 public:
  UnsafeStack(const size_t cap)
      : linear_allocator_(this + 1, sizeof(ElemT) * cap), cap_(cap) {}
  
  ~UnsafeStack() {
    while (!IsEmpty()) {
      Pop();
    }
  }

  template<typename... ArgsT>
  bool TryPush(ArgsT&&... args) {
    ElemT* allocated = AllocateForPush();
    if (allocated == nullptr) {
      return false;
    }
    new (allocated) ElemT(std::forward<ArgsT>(args)...);
    ++size_;
    return true;
  }

  ElemT& Top() {
    if (size_ == 0) {
      throw std::logic_error(UNABLE_TO_TOP_);
    }
    return *static_cast<ElemT*>(linear_allocator_.Data(sizeof(ElemT) * (size_ - 1)));
  }

  ElemT& Bottom() {
    if (size_ == 0) {
      throw std::logic_error(UNABLE_TO_BOTTOM_);
    }
    return *static_cast<ElemT*>(linear_allocator_.Data());
  }

  void Pop() {
    if (size_ == 0) {
      throw std::logic_error(UNABLE_TO_POP_);
    }
    (&Top())->~ElemT();
    linear_allocator_.Deallocate(sizeof(ElemT));
    --size_;
  }

  size_t Size() {
    return size_;
  }

  bool IsEmpty() {
    return size_ == 0;
  }

  static size_t CalcSize(const size_t cap) {
    return sizeof(UnsafeStack<ElemT>) + cap * sizeof(ElemT);
  }

 private:
  ElemT* AllocateForPush() {
    return static_cast<ElemT*>(linear_allocator_.Allocate(sizeof(ElemT)));
  }

 private:
  static const char* const UNABLE_TO_TOP_;
  static const char* const UNABLE_TO_POP_;
  static const char* const UNABLE_TO_BOTTOM_;

  LinearAllocator linear_allocator_;

  size_t size_ = 0;
  size_t cap_  = 0;

};

template<typename ElemT>
const char* const UnsafeStack<ElemT>::UNABLE_TO_TOP_ = "stack size is zero, unable to get top element";

template<typename ElemT>
const char* const UnsafeStack<ElemT>::UNABLE_TO_POP_ = "stack size is zero, unable to pop";

template<typename ElemT>
const char* const UnsafeStack<ElemT>::UNABLE_TO_BOTTOM_ = "unable to get element from bottom of empty stack";

#endif /* unsafe_stack.hpp */
