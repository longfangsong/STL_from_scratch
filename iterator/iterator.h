//
// Created by 龙方淞 on 2018/1/22.
//

#ifndef STL_FROM_SCRATCH_ITERATOR_H
#define STL_FROM_SCRATCH_ITERATOR_H

#include <cstddef>
#include "./iterator_traits.h"
#include "../memory/memory.h"

namespace Readable {
    template<
            typename Category,
            typename T,
            typename Distance = std::ptrdiff_t,
            typename Pointer = T *,
            typename Reference = T &
    >
    struct iterator {
        // 以下是iterator_traits所需知道的内容
        typedef Category iterator_category;
        typedef T value_type;
        typedef Distance difference_type;
        typedef Pointer pointer;
        typedef Reference reference;
    };

    template<typename Iterator>
    class reverse_iterator : public Readable::iterator<
            typename Readable::iterator_traits<Iterator>::iterator_category,
            typename Readable::iterator_traits<Iterator>::value_type,
            typename Readable::iterator_traits<Iterator>::difference_type,
            typename Readable::iterator_traits<Iterator>::pointer,
            typename Readable::iterator_traits<Iterator>::reference> {
    protected: // 虽然不知道意义何在，但根据标准文件，current是protected的
        Iterator current;
    public:
        typedef Iterator iterator_type;
        typedef reverse_iterator<Iterator> self_type;
        typedef typename iterator_traits<Iterator>::difference_type difference_type;
        typedef typename iterator_traits<Iterator>::reference reference;
        typedef typename iterator_traits<Iterator>::pointer pointer;

        reverse_iterator() = default;

        explicit reverse_iterator(Iterator x) : current(x) {}

        template<typename T>
        reverse_iterator(const reverse_iterator<T> &other) :current(other.current) {}

        template<typename T>
        reverse_iterator &operator=(const reverse_iterator<T> &other) {
            current = other.current;
            return *this;
        }

        Iterator base() const {
            return current;
        }

        reference operator*() const {
            Iterator tmp = current;
            return *--tmp;
        }

        pointer operator->() const {
            return Readable::addressof(operator*());
        }

        reference operator[](difference_type n) const { return *(*this + n); }

        self_type &operator++() {
            --current;
            return *this;
        }

        self_type &operator--() {
            ++current;
            return *this;
        }

        self_type operator++(int) {
            self_type temp = *this;
            --current;
            return temp;
        }

        self_type operator--(int) {
            self_type temp = *this;
            ++current;
            return temp;
        }

        self_type operator+(difference_type n) const {
            return self_type(current - n);
        }

        self_type operator-(difference_type n) const {
            return self_type(current + n);
        }

        self_type &operator+=(difference_type n) {
            current -= n;
            return *this;
        }

        self_type &operator-=(difference_type n) {
            current += n;
            return *this;
        }
    };

    template<typename Iterator1, typename Iterator2>
    bool operator==(const reverse_iterator<Iterator1> &lhs,
                    const reverse_iterator<Iterator2> &rhs) {
        return lhs.base() == rhs.base();
    };

    template<typename Iterator1, typename Iterator2>
    bool operator!=(const reverse_iterator<Iterator1> &lhs,
                    const reverse_iterator<Iterator2> &rhs) {
        return lhs.base() != rhs.base();
    };

    template<typename Iterator1, typename Iterator2>
    bool operator>(const reverse_iterator<Iterator1> &lhs,
                   const reverse_iterator<Iterator2> &rhs) {
        return lhs.base() < rhs.base();
    };

    template<typename Iterator1, typename Iterator2>
    bool operator<(const reverse_iterator<Iterator1> &lhs,
                   const reverse_iterator<Iterator2> &rhs) {
        return lhs.base() > rhs.base();
    };

    template<typename Iterator1, typename Iterator2>
    bool operator<=(const reverse_iterator<Iterator1> &lhs,
                    const reverse_iterator<Iterator2> &rhs) {
        return lhs.base() >= rhs.base();
    };

    template<typename Iterator1, typename Iterator2>
    bool operator>=(const reverse_iterator<Iterator1> &lhs,
                    const reverse_iterator<Iterator2> &rhs) {
        return lhs.base() <= rhs.base();
    };
};
#endif //STL_FROM_SCRATCH_ITERATOR_H
