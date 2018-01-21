//
// Created by 龙方淞 on 2018/1/20.
//

#ifndef STL_FROM_SCRATCH_IS_INTEGRAL_H
#define STL_FROM_SCRATCH_IS_INTEGRAL_H

#include "./integral_constant.h"
#include "./remove_cv.h"

namespace Readable {
    template<typename T>
    struct is_integral_without_cv : public false_type {
    };
    template<>
    struct is_integral_without_cv<bool> : public true_type {
    };
    template<>
    struct is_integral_without_cv<char> : public true_type {
    };
    template<>
    struct is_integral_without_cv<signed char> : public true_type {
    };
    template<>
    struct is_integral_without_cv<unsigned char> : public true_type {
    };
    template<>
    struct is_integral_without_cv<wchar_t> : public true_type {
    };
    template<>
    struct is_integral_without_cv<char16_t> : public true_type {
    };
    template<>
    struct is_integral_without_cv<char32_t> : public true_type {
    };
    template<>
    struct is_integral_without_cv<short> : public true_type {
    };
    template<>
    struct is_integral_without_cv<unsigned short> : public true_type {
    };
    template<>
    struct is_integral_without_cv<int> : public true_type {
    };
    template<>
    struct is_integral_without_cv<unsigned int> : public true_type {
    };
    template<>
    struct is_integral_without_cv<long> : public true_type {
    };
    template<>
    struct is_integral_without_cv<unsigned long> : public true_type {
    };
    template<>
    struct is_integral_without_cv<long long> : public true_type {
    };
    template<>
    struct is_integral_without_cv<unsigned long long> : public true_type {
    };
    template<class T>
    struct is_integral : public is_integral_without_cv<typename Readable::remove_cv<T>::type> {
    };
};
#endif //STL_FROM_SCRATCH_IS_INTEGRAL_H
