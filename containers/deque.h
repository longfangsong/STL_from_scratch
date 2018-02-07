//
// Created by longfangsong on 18-2-6.
//

#ifndef STL_FROM_SCRATCH_DEQUE_H
#define STL_FROM_SCRATCH_DEQUE_H

#include <cstddef>
#include "../memory/allocator.h"
#include "../iterator/iterator.h"

namespace Readable {
    template<typename T, typename Allocator, size_t part_size>
    struct deque_part {
        T *part_space_start;
        // 与常规容器不同，此处取全闭区间
        T *part_start;
        T *part_end;

        deque_part() : part_space_start(Allocator::allocate(part_size)), part_start(nullptr), part_end(nullptr) {}
    };

    template<typename T, typename ReferenceType, typename PointerType, size_t PartSize>
    class deque_iterator : public Readable::iterator<
            Readable::random_access_iterator_tag,
            T,
            std::ptrdiff_t,
            PointerType,
            ReferenceType
    > {
    public:
        // 为方便起见定义的一些类型
        // 本身的类型
        typedef deque_iterator<T, ReferenceType, PointerType, PartSize> self_type;
        // 正常的迭代器类型
        typedef deque_iterator<T, T &, T *, PartSize> iterator_type;
        // const的迭代器类型
        typedef deque_iterator<T, const T &, const T *, PartSize> const_iterator_type;

        // 实现Iterator concept
        ReferenceType operator*() const {
            return *the_object;
        }

        PointerType operator->() const {
            return the_object;
        }

        self_type &operator++() {
            if (the_object == part_now_in->part_end) {
                // 由于已知part连续存放，++part_now_in即可得到上一个part
                ++part_now_in;
                the_object = part_now_in->part_start;
            } else {
                ++the_object;
            }
            return *this;
        }

        // 实现ForwardIterator concept
        self_type operator++(int) {
            self_type origin_this = *this;
            ++(*this);
            return origin_this;
        }

        // 实现BidirectionalIterator concept
        self_type &operator--() {
            if (the_object == part_now_in->part_start) {
                --part_now_in;
                the_object = part_now_in->part_end;
            } else {
                --the_object;
            }
            return *this;
        }

        self_type operator--(int) {
            self_type origin_this = *this;
            --(*this);
            return origin_this;
        }

        // 实现RandomAccessIterator concept
        self_type &operator+=(ptrdiff_t n) {
            if (n < 0) {
                return operator-=(-n);
            } else if (n <= part_now_in->part_end - the_object) {
                // 目标和目前位置在同一区间，直接前进
                the_object += n;
            } else {
                // 目标位置和目前位置在不同区间
                // 先前进到当前区间的最后
                // 注意由于目标位置一定在目前区间之后的区间中， the_object 后方的元素一定是填满的
                n -= part_now_in->part_end - the_object;
                size_t parts_to_advance = n / PartSize;
                part_now_in += parts_to_advance;
                n -= parts_to_advance * PartSize;
                if (n == 0) {
                    // 恰好前进到合适的位置
                    the_object = part_now_in->part_end;
                } else {
                    // 还剩余一些位置需要前进
                    ++part_now_in;
                    the_object = part_now_in->part_start + n;
                }
            }
            return *this;
        }

        self_type &operator-=(ptrdiff_t n) {
            if (n < 0) {
                return operator+=(-n);
            } else if (n <= the_object - part_now_in->part_start) {
                // 目标和目前位置在同一区间，直接后退
                the_object -= n;
            } else {
                // 目标位置和目前位置在不同区间
                // 先后退到当前区间的开始
                // 注意由于目标位置一定在目前区间之前的区间中， the_object 前方的元素一定是填满的
                n -= the_object - part_now_in->part_start;
                size_t parts_to_back = n / PartSize;
                part_now_in -= parts_to_back;
                n -= parts_to_back * PartSize;
                if (n == 0) {
                    // 恰好后退到合适的位置
                    the_object = part_now_in->part_start;
                } else {
                    // 还剩余一些位置需要后退
                    --part_now_in;
                    the_object = part_now_in->part_end - n;
                }
            }
            return *this;
        }

        template<typename T, typename ReferenceTypeA, typename PointerTypeA, size_t PartSize,
                typename ReferenceTypeB, typename PointerTypeB>
        friend ptrdiff_t operator-(deque_iterator<T, ReferenceTypeA, PointerTypeA, PartSize> a,
                                   deque_iterator<T, ReferenceTypeB, PointerTypeB, PartSize> b) {
            if (a.part_now_in == b.part_now_in) {
                return static_cast<ptrdiff_t>(a.the_object - b.the_object);
            } else if (a.part_now_in < b.part_now_in) {
                return -(b - a);
            }
            // 保证a、b处于不同的parts中，且a在b之后
            return static_cast<ptrdiff_t>(a.part_now_in - b.part_now_in - 1) * PartSize +
                   static_cast<ptrdiff_t>(a.part_now_in->part_end - a.the_object) +
                   static_cast<ptrdiff_t>(b.the_object - b.part_now_in->part_start);
        }

        ReferenceType operator[](size_t n) {
            return *(*this + n);
        }

        friend bool operator<(const deque_iterator &lhs, const deque_iterator &rhs) {
            if (lhs.part_now_in < rhs.part_now_in) {
                return true;
            } else if (lhs.part_now_in > rhs.part_now_in) {
                return false;
            }
            return lhs.the_object < rhs.the_object;
        }

        friend bool operator>(const deque_iterator &lhs, const deque_iterator &rhs) {
            return rhs < lhs;
        }

        friend bool operator<=(const deque_iterator &lhs, const deque_iterator &rhs) {
            return !(rhs < lhs);
        }

        friend bool operator>=(const deque_iterator &lhs, const deque_iterator &rhs) {
            return !(lhs < rhs);
        }

        bool operator==(const deque_iterator &rhs) const {
            return the_object == rhs.the_object && part_now_in == rhs.part_now_in;
        }

        bool operator!=(const deque_iterator &rhs) const {
            return !(rhs == *this);
        }

    private:
        T *the_object;
        deque_part *part_now_in;
    };

    template<typename T, typename ReferenceType, typename PointerType, size_t PartSize>
    deque_iterator<T, ReferenceType, PointerType, PartSize>
    operator+(deque_iterator<T, ReferenceType, PointerType, PartSize> it, ptrdiff_t n) {
        it += n;
        return it;
    }

    template<typename T, typename ReferenceType, typename PointerType, size_t PartSize>
    deque_iterator<T, ReferenceType, PointerType, PartSize>
    operator+(ptrdiff_t n, deque_iterator<T, ReferenceType, PointerType, PartSize> it) {
        it += n;
        return it;
    }

    template<typename T, typename ReferenceType, typename PointerType, size_t PartSize>
    deque_iterator<T, ReferenceType, PointerType, PartSize>
    operator-(deque_iterator<T, ReferenceType, PointerType, PartSize> it, ptrdiff_t n) {
        it -= n;
        return it;
    }

    template<typename T, typename Allocator, size_t part_size = 8>
    class deque {
    public:
        typedef T value_type;
        typedef Allocator allocator_type;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef value_type &reference;
        typedef const value_type &const_reference;
        typedef Allocator::pointer pointer;
        typedef Allocator::const_pointer const_pointer;
        typedef deque_iterator<T, T &, T *, part_size> iterator;
        typedef deque_iterator<T, const T &, const T *, part_size> const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    private:

    public:
        explicit deque(const Allocator &alloc = Allocator()) {

        }
    };
}
#endif //STL_FROM_SCRATCH_DEQUE_H
