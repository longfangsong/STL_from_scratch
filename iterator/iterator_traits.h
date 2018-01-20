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

    /**
     * advance针对随机访问迭代器的优化实现
     * @tparam RandomAccessIterator 随机访问迭代器类型
     * @param it 要进行前进操作的迭代器
     * @param n 前进的步数，可正可负
     * @note 时间复杂度O(1)
     */
    template<typename RandomAccessIterator>
    void advance(RandomAccessIterator &it, typename iterator_traits<RandomAccessIterator>::difference_type n,
                 random_access_iterator_tag) {
        it += n;
    };

    /**
     * advance针对双向迭代器的优化实现
     * @tparam BidirectionalIterator 双向迭代器类型
     * @param it 要进行前进操作的迭代器
     * @param n 前进的步数，可正可负
     * @note 时间复杂度O(abs(n))
     */
    template<typename BidirectionalIterator>
    void advance(BidirectionalIterator &it, typename iterator_traits<BidirectionalIterator>::difference_type n,
                 bidirectional_iterator_tag) {
        if (n > 0) {
            for (typename iterator_traits<BidirectionalIterator>::difference_type i = 0; i < n; ++i) {
                ++it;
            }
        } else {
            for (typename iterator_traits<BidirectionalIterator>::difference_type i = 0; i < -n; ++i) {
                --it;
            }
        }
    };

    /**
     * advance的一般实现
     * @tparam InputIterator 任何符合InputIterator的类型
     * @param it 要进行前进操作的迭代器
     * @param n 前进的步数，只能是正数
     * @note 时间复杂度O(n)
     */
    template<typename InputIterator>
    void advance(InputIterator &it, typename iterator_traits<InputIterator>::difference_type n, input_iterator_tag) {
        for (typename iterator_traits<InputIterator>::difference_type i = 0; i < n; ++i) {
            ++it;
        }
    };


    /**
     * 对迭代器 @arg it 进行前进操作
     * @tparam InputIt 任何符合InputIterator的类型
     * @param it 要进行前进操作的迭代器
     * @param n 前进的步数
     */
    template<typename InputIt>
    void advance(InputIt &it, typename iterator_traits<InputIt>::difference_type n) {
        // 此处使用了traits技法
        // 先利用iterator_traits提取出iterator的类型
        // 再根据提取出的类型匹配到对应的实现
        advance(it, n, typename iterator_traits<InputIt>::iterator_category());
    };

    /**
     * 返回 @arg it 前进 @arg n 步后的迭代器
     * @tparam ForwardIt 前向迭代器以上
     * @param it 要前进迭代器
     * @param n 要前进的步数
     * @return @arg it 前进 @arg n 步后的迭代器
     */
    template<typename ForwardIt>
    ForwardIt next(ForwardIt it, typename Readable::iterator_traits<ForwardIt>::difference_type n = 1) {
        advance(it, n);
        return it;
    }

    /**
     * 返回两个迭代器之间的距离
     * @tparam InputIt 输入迭代器以上
     * @param first 第一个迭代器
     * @param last 最后一个迭代器
     * @return @arg first 和 @arg last 之间的距离
     */
    template<typename InputIt>
    typename Readable::iterator_traits<InputIt>::difference_type distance(InputIt first, InputIt last) {
        // traits技法，见advance的实现
        return distance(first, last, Readable::iterator_traits<InputIt>::iterator_category());
    }

    template<typename InputIt>
    typename Readable::iterator_traits<InputIt>::difference_type
    distance(InputIt first, InputIt last, input_iterator_tag) {
        typename Readable::iterator_traits<InputIt>::difference_type n = 0;
        while (first != last) {
            ++first;
            ++n;
        }
        return n;
    }

    template<typename RandomAccessIt>
    typename Readable::iterator_traits<RandomAccessIt>::difference_type
    distance(RandomAccessIt first, RandomAccessIt last, random_access_iterator_tag) {
        return last - first;
    }
}


#endif //STL_FROM_SCRATCH_ITERATOR_TRAITS_H
