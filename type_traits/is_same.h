//
// Created by 龙方淞 on 2018/1/20.
//

#ifndef STL_FROM_SCRATCH_IS_SAME_H
#define STL_FROM_SCRATCH_IS_SAME_H

#include "integral_constant.h"

namespace Readable {
    template<class T, class U>
    struct is_same : public Readable::false_type {
    };

    template<class T>
    struct is_same<T, T> : public Readable::true_type {
    };
};
#endif //STL_FROM_SCRATCH_IS_SAME_H
