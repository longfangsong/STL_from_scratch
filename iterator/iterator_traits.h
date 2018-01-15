//
// Created by 龙方淞 on 2017/12/19.
//

#ifndef STL_FROM_SCRATCH_ITERATOR_TRAITS_H
#define STL_FROM_SCRATCH_ITERATOR_TRAITS_H

#include <cstddef>
#include <iterator>

namespace Readable {
    /**
     * 各种类型iterator的tag，用于确定迭代器的类型
     */
    struct input_iterator_tag {
    };
    struct output_iterator_tag {
    };
    struct forward_iterator_tag : public input_iterator_tag {
    };
    struct bidirectional_iterator_tag : public forward_iterator_tag {
    };
    struct random_access_iterator_tag : public bidirectional_iterator_tag {
    };
    /**
     * 最普通的iterator traits，迭代器的信息由迭代器自己的实现给出
     * @tparam Iterator 一个容器内部实现的迭代器
     */
    template<typename Iterator>
    struct iterator_traits {
        // 以下几个内部类型都是C++标准规定要实现的
        // 其作用是在STL算法等调用过程中可以获得迭代器所指类型的信息
        typedef typename Iterator::difference_type difference_type;
        typedef typename Iterator::value_type value_type;
        typedef typename Iterator::pointer pointer;
        typedef typename Iterator::reference reference;
        typedef typename Iterator::iterator_category iterator_category;
    };
    /**
     * iterator traits针对原生指针的特化，迭代器的信息已经知道了
     */
    template<typename T>
    struct iterator_traits<T *> {
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef T *pointer;
        typedef T &reference;
        typedef Readable::random_access_iterator_tag iterator_category;
    };
    /**
     * iterator traits针对原生pointer-to-const的特化，迭代器的信息已经知道了
     */
    template<typename T>
    struct iterator_traits<const T *> {
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T *pointer;
        typedef const T &reference;
        typedef Readable::random_access_iterator_tag iterator_category;
    };
}


#endif //STL_FROM_SCRATCH_ITERATOR_TRAITS_H
