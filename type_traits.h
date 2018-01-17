//
// Created by 龙方淞 on 2018/1/17.
//

#ifndef STL_FROM_SCRATCH_TYPE_TRAITS_H
#define STL_FROM_SCRATCH_TYPE_TRAITS_H
namespace Readable {
    template<typename T, T v>
    struct integral_constant {
        typedef T value_type;
        typedef Readable::integral_constant<T, v> type;
    };
    typedef Readable::integral_constant<bool, true> true_type;
    typedef Readable::integral_constant<bool, false> false_type;

    template<typename T>
    struct is_integral : public false_type {
    };
    template<>
    struct is_integral<bool> : public true_type {
    };
    template<>
    struct is_integral<char> : public true_type {
    };
    template<>
    struct is_integral<signed char> : public true_type {
    };
    template<>
    struct is_integral<unsigned char> : public true_type {
    };
    template<>
    struct is_integral<wchar_t> : public true_type {
    };
    template<>
    struct is_integral<char16_t> : public true_type {
    };
    template<>
    struct is_integral<char32_t> : public true_type {
    };
    template<>
    struct is_integral<short> : public true_type {
    };
    template<>
    struct is_integral<unsigned short> : public true_type {
    };
    template<>
    struct is_integral<int> : public true_type {
    };
    template<>
    struct is_integral<unsigned int> : public true_type {
    };
    template<>
    struct is_integral<long> : public true_type {
    };
    template<>
    struct is_integral<unsigned long> : public true_type {
    };
    template<>
    struct is_integral<long long> : public true_type {
    };
    template<>
    struct is_integral<unsigned long long> : public true_type {
    };
};
#endif //STL_FROM_SCRATCH_TYPE_TRAITS_H
