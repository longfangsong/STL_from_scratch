//
// Created by 龙方淞 on 2018/1/20.
//

#ifndef STL_FROM_SCRATCH_REMOVE_CV_H
#define STL_FROM_SCRATCH_REMOVE_CV_H
namespace Readable {
    template<typename T>
    struct remove_const {
        typedef T type;
    };
    template<typename T>
    struct remove_const<const T> {
        typedef T type;
    };

    template<typename T>
    struct remove_volatile {
        typedef T type;
    };
    template<typename T>
    struct remove_volatile<volatile T> {
        typedef T type;
    };
    template<class T>
    struct remove_cv {
        typedef typename Readable::remove_volatile<typename Readable::remove_const<T>::type>::type type;
    };
};
#endif //STL_FROM_SCRATCH_REMOVE_CV_H
