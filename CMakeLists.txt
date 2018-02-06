cmake_minimum_required(VERSION 3.5)
project(STL_from_scratch)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unused-variable")
set(SOURCE_FILES main.cpp memory/allocator.h memory/uninitialized_memory_functions.h iterator/iterator_traits.h algorithm/modifying_sequence.h containers/forward_list.h utility/utility.h type_traits/type_traits.h type_traits/integral_constant.h type_traits/is_integral.h type_traits/remove_cv.h type_traits/is_same.h memory/memory.h containers/vector.h iterator/iterator.h algorithm/algorithm.h)
add_executable(STL_from_scratch ${SOURCE_FILES})