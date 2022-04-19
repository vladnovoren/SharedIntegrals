cmake_minimum_required(VERSION 3.16.3 FATAL_ERROR)

project(SharedIntegrals)

# find_package(Boost COMPONENTS system filesystem)

# add_executable(test src/test.cpp)

add_library(shared_memory STATIC
  src/linear_allocator.cpp
  src/fd_helpers.cpp
  src/managed_shared_memory.cpp
  src/read_add_addresses_map.cpp
  src/sem_lock_guard.cpp
  src/semaphore.cpp
  src/shared_memory_object.cpp
)

add_compile_options(
  # -fsanitize=address
  -g

  -Wall
  -Wextra
  -Wpedantic
  -Werror
)

add_link_options(
  # -fsanitize=address
  -g

  -Wall
  -Wextra
  -Wpedantic
  -Werror
)

target_link_libraries(shared_memory PUBLIC -lrt -pthread)
target_include_directories(shared_memory PUBLIC include/)

add_executable(test_managed_shared_memory src/test_managed_shared_memory.cpp)
target_link_libraries(test_managed_shared_memory PUBLIC shared_memory)

# add_executable(test_fork_shared_memory src/test_fork_shared_memory.cpp)
# target_link_libraries(test_fork_shared_memory PUBLIC shared_memory)
# target_link_libraries(test PUBLIC -lrt -pthread)
# target_link_libraries(test PUBLIC Boost::system Boost::filesystem)
