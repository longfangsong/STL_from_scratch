//
// Created by 龙方淞 on 2018/1/21.
//

#ifndef STL_FROM_SCRATCH_MEMORY_H
#define STL_FROM_SCRATCH_MEMORY_H

#include "./allocator.h"

namespace Readable {
    template<typename T>
    T *addressof(T &arg) {
        return reinterpret_cast<T *>(&const_cast<char &>(reinterpret_cast<const volatile char &>(arg)));
    }
};

#include "./uninitialized_memory_functions.h"

#endif //STL_FROM_SCRATCH_MEMORY_H
