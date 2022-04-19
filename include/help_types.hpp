#ifndef HELP_TYPES_HPP
#define HELP_TYPES_HPP

struct create_only_t {};
struct find_only_t {};

static const create_only_t create_only;
static const find_only_t find_only;

template<typename ObjT>
struct Destructor {
  Destructor(ObjT* obj) : obj_(obj) {
    assert(obj != nullptr);
  }

  ~Destructor() {
    obj_->~ObjT();
  }

 private:
  ObjT* obj_;

};

#endif /* help_types.hpp */
