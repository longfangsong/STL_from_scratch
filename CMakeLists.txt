cmake_minimum_required(VERSION 3.7)
project(STL_from_scratch)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp memory/allocator.h memory/uninitialized_memory_functions.h iterator/iterator_traits.h algorithm/modifying_sequence.h containers/_forward_list.h containers/forward_list.h utility/utility.h type_traits.h)
add_executable(STL_from_scratch ${SOURCE_FILES})