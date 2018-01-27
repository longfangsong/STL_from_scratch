//
// Created by 龙方淞 on 2017/12/18.
//

#ifndef STL_FROM_SCRATCH_MEMORY_FUNCTIONS_H
#define STL_FROM_SCRATCH_MEMORY_FUNCTIONS_H

#include "../iterator/iterator_traits.h"
#include "./memory.h"

namespace Readable {
    /**
     * 将 [@arg first,@arg last) 之间的元素复制到未初始化过的@arg desination_first开始的地址中
     * @tparam ForwardIt 符合InpytIterator要求的迭代器
     * @tparam InputIt 符合ForwardIterator要求的迭代器
     * @param first 要复制序列的头迭代器
     * @param last 要复制序列的超尾迭代器
     * @param desination_first 复制目标的头迭代器
     * @return 复制完成的序列的超尾迭代器
     */
    template<typename InputIt, typename ForwardIt>
    ForwardIt uninitialized_copy(InputIt first, InputIt last, ForwardIt desination_first) {
        // 注释一个，另外几个同理
        typedef typename Readable::iterator_traits<ForwardIt>::value_type Value;
        // 现在复制到的位置
        ForwardIt current = desination_first;
        try {
            // 复制
            for (; first != last; ++first, ++current) {
                // 用new运算符复制元素
                // addressof即取地址
                // 不用&为的是防止用户重载&运算符，造成&意义改变
                ::new(static_cast<void *>(Readable::addressof(*current))) Value(*first);
            }
            return current;
        } catch (...) {
            // 构造失败
            // 根据官方文档，需要回滚操作
            for (; desination_first != current; ++desination_first) {
                desination_first->~Value();
            }
            throw;
        }
    }

    /**
     * 将 [@arg first,@arg last) 之间的元素移动到未初始化过的@arg desination_first开始的地址中
     * @tparam ForwardIt 符合InpytIterator要求的迭代器
     * @tparam InputIt 符合ForwardIterator要求的迭代器
     * @param first 要move序列的头迭代器
     * @param last 要move序列的超尾迭代器
     * @param desination_first move目标的头迭代器
     * @return move完成的序列的超尾迭代器
     */
    template<typename InputIt, typename ForwardIt>
    ForwardIt uninitialized_move(InputIt first, InputIt last, ForwardIt desination_first) {
        typedef typename Readable::iterator_traits<ForwardIt>::value_type Value;
        ForwardIt current = desination_first;
        try {
            for (; first != last; ++first, ++current) {
                ::new(static_cast<void *>(Readable::addressof(*current))) Value(std::move(*first));
            }
            return current;
        } catch (...) {
            for (; desination_first != current; ++desination_first) {
                desination_first->~Value();
            }
            throw;
        }
    };

    /**
     * 将未初始化过的地址[@arg first,@arg last) 之间的空间用@arg value填充
     * @tparam ForwardIt 符合InpytIterator要求的迭代器
     * @tparam T 要填充的元素类型
     * @param first 要填充序列的头迭代器
     * @param last 要填充序列的超尾迭代器
     * @param value 要填充的值
     */
    template<typename ForwardIt, typename T>
    void uninitialized_fill(ForwardIt first, ForwardIt last, const T &value) {
        typedef typename std::iterator_traits<ForwardIt>::value_type Value;
        ForwardIt current = first;
        try {
            for (; current != last; ++current) {
                ::new(static_cast<void *>(Readable::addressof(*current))) Value(value);
            }
        } catch (...) {
            for (; first != current; ++first) {
                first->~Value();
            }
            throw;
        }
    }

    /**
     * 将由@arg first开始的@arg count个T元素复制到未初始化过的@arg desination_first开始的地址中
     * @tparam ForwardIt 符合InpytIterator要求的迭代器
     * @tparam Size 值类型
     * @tparam InputIt 符合ForwardIterator要求的迭代器
     * @param first 要复制序列的头迭代器
     * @param count 要复制的元素个数
     * @param desination_first 复制目标的头迭代器
     * @return 复制完成的序列的超尾迭代器
     */
    template<typename InputIt, typename Size, typename ForwardIt>
    ForwardIt uninitialized_copy_n(InputIt first, Size count, ForwardIt desination_first) {
        typedef typename std::iterator_traits<ForwardIt>::value_type Value;
        ForwardIt current = desination_first;
        try {
            for (; count > 0; ++first, ++current, --count) {
                ::new(static_cast<void *>(Readable::addressof(*current))) Value(*first);
            }
        } catch (...) {
            for (; desination_first != current; ++desination_first) {
                desination_first->~Value();
            }
            throw;
        }
        return current;
    }

    /**
     * 将以@arg first开始的@arg n个sizeof(T)大小的未初始化过的地址之间的空间用@arg value填充
     * @tparam ForwardIt 符合InpytIterator要求的迭代器
     * @tparam Size 值类型
     * @tparam T 要填充的元素类型
     * @param first 要填充序列的头迭代器
     * @param count 要填充的元素个数
     * @param value 要填充的值
     * @return 指向复制的最后一个元素的下一个元素的迭代器
     */
    template<typename ForwardIt, typename Size, typename T>
    ForwardIt uninitialized_fill_n(ForwardIt first, Size count, const T &value) {
        typedef typename std::iterator_traits<ForwardIt>::value_type Value;
        ForwardIt current = first;
        try {
            for (; count > 0; ++current, --count) {
                ::new(static_cast<void *>(Readable::addressof(*current))) Value(value);
            }
            return current;
        } catch (...) {
            for (; first != current; ++first) {
                first->~Value();
            }
            throw;
        }
    }

    /**
     * 在 @arg p 处以 @arg construct_param 为参数构造一个对象
     * @tparam TypeToConstruct 要构造的对象类型
     * @tparam ConstructorParamType 单参数构造函数的参数类型
     * @param p 指向要构造位置的指针
     * @param construct_param 构造函数参数
     */
    template<typename TypeToConstruct, typename ConstructorParamType>
    inline void construct(TypeToConstruct *p, const ConstructorParamType &construct_param) {
        new((void *) p) TypeToConstruct(construct_param);
    }

    /**
     * 在@arg p 处构造一个对象
     * @tparam TypeToConstruct 要构造的对象类型
     * @param p 指向要构造位置的指针
     */
    template<typename TypeToConstruct>
    inline void construct(TypeToConstruct *p) {
        new((void *) p) TypeToConstruct();
    }

    /**
     * 析构 @arg p 处的一个对象
     * @tparam T 要析构的对象类型
     * @param p 指向要析构的对象的指针
     */
    template<typename T>
    void destroy(T *p) {
        // 这里的T可以是基本数据类型
        // 感谢C++标准，我们可以直接对基本数据类型进行"析构"，而无需使用traits
        // 见标准文件 12.4 p15
        p->~T();
    }

    // 同destroy，since C++17
    template<typename T>
    inline void destroy_at(T *p) {
        Readable::destroy(p);
    }

    /**
     * 析构从 [@arg first,@arg last) 之间的所有对象
     * @tparam ForwardIt 迭代器
     * @param first 头迭代器
     * @param last 超尾迭代器
     */
    template<typename ForwardIt>
    void destroy(ForwardIt first, ForwardIt last) {
        for (; first != last; ++first) {
            Readable::destroy_at(std::addressof(*first));
        }
    }

}
#endif //STL_FROM_SCRATCH_MEMORY_FUNCTIONS_H
