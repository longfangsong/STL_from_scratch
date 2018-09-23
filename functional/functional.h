//
// Created by 龙方淞 on 2018/9/19.
//

#ifndef STL_FROM_SCRATCH_FUNCTIONAL_H
#define STL_FROM_SCRATCH_FUNCTIONAL_H
namespace Readable {
    template<class Arg1Type, class Arg2Type, class ResultType>
    struct binary_function {
        typedef Arg1Type first_argument_type;
        typedef Arg2Type second_argument_type;
        typedef ResultType result_type;
    };

    template<typename T=void>
    struct less : binary_function<T, T, bool> {
        bool operator()(const T &x, const T &y) const {
            return x < y;
        }
    };
}
#endif //STL_FROM_SCRATCH_FUNCTIONAL_H
