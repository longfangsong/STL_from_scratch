//
// Created by 龙方淞 on 2018/1/20.
//

#ifndef STL_FROM_SCRATCH_INTEGRAL_CONSTANT_H
#define STL_FROM_SCRATCH_INTEGRAL_CONSTANT_H
namespace Readable {
    template<typename T, T v>
    struct integral_constant {
        typedef T value_type;
        typedef Readable::integral_constant<T, v> type;
        static const T value = v;
    };
    typedef Readable::integral_constant<bool, true> true_type;
    typedef Readable::integral_constant<bool, false> false_type;
};
#endif //STL_FROM_SCRATCH_INTEGRAL_CONSTANT_H
