cmake_minimum_required(VERSION 3.16.3 FATAL_ERROR)

project(SharedIntegrals)

# find_package(Boost COMPONENTS system filesystem)

# add_executable(test src/test.cpp)

add_library(linear_shm_allocator STATIC
  src/shm_linear_allocator.cpp
  src/fd_helpers.cpp
)

add_compile_options(
  -fsanitize=address
  -g

  -Wall
  -Wextra
  -Werror
)

add_link_options(
  -fsanitize=address
  -g

  -Wall
  -Wextra
  -Werror
)

target_link_libraries(linear_shm_allocator PUBLIC -lrt)
target_include_directories(linear_shm_allocator PUBLIC include/)

add_executable(test_lsa src/test_shm_linear_allocator.cpp)
target_link_libraries(test_lsa PUBLIC linear_shm_allocator)

# target_link_libraries(test PUBLIC -lrt -pthread)
# target_link_libraries(test PUBLIC Boost::system Boost::filesystem)
