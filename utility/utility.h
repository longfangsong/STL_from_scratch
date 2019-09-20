#ifndef STL_FROM_SCRATCH_UTILITY_H
#define STL_FROM_SCRATCH_UTILITY_H
namespace Readable {
    template<typename T1, typename T2>
    struct pair {
        T1 first;
        T2 second;

        pair(const T1 &_first, const T2 &_second) : first(_first), second(_second) {}
    };

    template<typename T1, typename T2>
    pair<T1, T2> make_pair(const T1 &first, const T2 &second) {
        return pair<T1, T2>(first, second);
    };

};
#endif //STL_FROM_SCRATCH_UTILITY_H
