//
// Created by 龙方淞 on 2017/12/14.
//

#ifndef STL_FROM_SCRATCH_L_ALLOCATOR_H
#define STL_FROM_SCRATCH_L_ALLOCATOR_H

#include <cstddef>
#include <new>
#include <iostream>

namespace Readable {
    template<typename T>
    struct allocator {
        typedef T value_type;
        typedef T *pointer;
        typedef const T *const_pointer;
        typedef T &reference;
        typedef const T &const_reference;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;

        // 赋予空间配置器为另一种类型的变量配置空间的能力
        // 这样做是为了方便list等基于Node储存变量的容器为其Node分配空间
        template<typename U>
        struct rebind {
            typedef allocator<U> other;
        };

        allocator() = default;

        allocator(const allocator &other) = default;

        template<typename U>
        allocator(const allocator<U> &other) {
        }

        ~allocator() = default;

        /**
         * 分配 @arg n * sizeof(T)个字节的内存
         * @param n 要分配的对象个数
         * @return 分配到的内存头指针
         */
        static inline T *allocate(std::size_t n) {
            return (T *) (::operator new(n * sizeof(T)));
        }

        /**
         * 回收 @arg n*sizeof(T) 个字节的内存
         * @param p 要回收的指针
         * @param n 要回收的指针内T类型元素的多少
         * @note 由于new-delete的实现，@arg n 实际上并没有用处
         */
        static inline void deallocate(T *p, std::size_t n) {
            ::operator delete(p);
        }
    };
}

#endif //STL_FROM_SCRATCH_L_ALLOCATOR_H
