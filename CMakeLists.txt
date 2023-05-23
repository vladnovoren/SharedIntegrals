cmake_minimum_required(VERSION 3.16.3 FATAL_ERROR)

project(SharedIntegrals)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)

find_package(GTest)

add_library(shared_memory STATIC
  src/linear_allocator.cpp
  src/fd_helpers.cpp
  src/managed_shared_memory.cpp
  src/read_add_addresses_map.cpp
  src/sem_lock_guard.cpp
  src/semaphore.cpp
  src/shared_memory_object.cpp
  src/children.cpp
)

add_library(integrals STATIC
  src/integral_task.cpp
  src/functions.cpp
)

add_compile_options(
  -g
)

add_link_options(
  -g
)

target_link_libraries(shared_memory PUBLIC -lrt -pthread)
target_include_directories(shared_memory PUBLIC include/)

target_link_libraries(integrals PUBLIC shared_memory)
target_include_directories(integrals PUBLIC include/)

add_executable(test_managed_shared_memory src/test_managed_shared_memory.cpp)
target_link_libraries(test_managed_shared_memory PUBLIC shared_memory)

add_executable(multi src/multi.cpp)
target_link_libraries(multi PUBLIC integrals)

add_executable(single src/single.cpp)
target_link_libraries(single PUBLIC integrals)

enable_testing()

add_executable(
  int_test
  src/test.cpp
)

target_link_libraries(
  int_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(int_test)
