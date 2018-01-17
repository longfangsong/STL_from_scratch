//
// Created by 龙方淞 on 2018/1/17.
//

#ifndef STL_FROM_SCRATCH_UTILITY_H
#define STL_FROM_SCRATCH_UTILITY_H
namespace Readable {
    template<class T1, class T2>
    struct pair {
        T1 first;
        T2 second;

        pair(const T1 &_first, const T2 &_second) : first(_first), second(_second) {}
    };
};
#endif //STL_FROM_SCRATCH_UTILITY_H
