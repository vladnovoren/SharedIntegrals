#ifndef CHILDREN_HPP
#define CHILDREN_HPP

#include <cstddef>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void MakeChildren(const size_t n);

void WaitChildren(const pid_t parent_id);

#endif /* children.hpp */
