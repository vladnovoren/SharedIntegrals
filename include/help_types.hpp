#ifndef HELP_TYPES_HPP
#define HELP_TYPES_HPP

struct create_only_t {};
struct find_only_t {};

static const create_only_t create_only;
static const find_only_t find_only;

static constexpr double eps = 1e-2;

template<typename ObjT>
struct Destructor {
  Destructor(ObjT* obj, pid_t creator_id) : obj_(obj), creator_id_(creator_id) {
    assert(obj != nullptr);
  }

  ~Destructor() {
    if (IsCreator()) {
      obj_->~ObjT();
    }
  }

 private:
  bool IsCreator() {
    return getpid() == creator_id_;
  }

 private:
  ObjT* obj_;
  pid_t creator_id_ = -1;

};

#endif /* help_types.hpp */
