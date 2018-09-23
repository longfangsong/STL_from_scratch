//
// Created by 龙方淞 on 2018/9/7.
//

#ifndef STL_FROM_SCRATCH_LIST_H
#define STL_FROM_SCRATCH_LIST_H

#include "../iterator/iterator.h"
#include "../type_traits/type_traits.h"
#include "../functional/functional.h"
#include "../utility/utility.h"
#include <cstdint>

namespace Readable {
    struct list_node_base {
        list_node_base *prev;
        list_node_base *next;
    };

    template<typename T>
    struct list_node : public list_node_base {
        T data;
    };

    template<typename T, typename ReferenceType, typename PointerType>
    struct list_iterator : public Readable::iterator<
            Readable::bidirectional_iterator_tag,
            T,
            std::ptrdiff_t,
            PointerType,
            ReferenceType
    > {
        // 为方便起见定义的一些类型
        // 本身的类型
        typedef list_iterator<T, ReferenceType, PointerType> self_type;
        // 正常的迭代器类型
        typedef list_iterator<T, T &, T *> iterator_type;
        // const的迭代器类型
        typedef list_iterator<T, const T &, const T *> const_iterator_type;

        list_node_base *node;

        list_iterator() = default;

        explicit list_iterator(list_node_base *n) : node(n) {}

        list_iterator(const const_iterator_type &other) : node(other.node) {}

        list_iterator(const iterator_type &other) : node(other.node) {}

        bool operator==(const self_type &other) const { return node == other.node; }

        bool operator!=(const self_type &other) const { return node != other.node; }

        ReferenceType operator*() const {
            return ((list_node<T> *) node)->data;
        }

        PointerType operator->() const {
            return Readable::addressof(operator*());
        }

        self_type &operator++() {
            node = node->next;
            return *this;
        }

        self_type operator++(int) {
            self_type origin_this = *this;
            node = node->next;
            return origin_this;
        }

        self_type &operator--() {
            node = node->prev;
            return *this;
        }

        self_type operator--(int) {
            self_type origin_this = *this;
            node = node->prev;
            return origin_this;
        }
    };


    template<class T, class Allocator = allocator <T> >
    class list final {
    public:
        typedef T value_type;
        typedef Allocator allocator_type;
        static_assert(Readable::is_same<typename allocator_type::value_type, value_type>::value,
                      "Allocator::value_type must be same type as value_type");
        typedef value_type &reference;
        typedef const value_type &const_reference;
        typedef list_iterator<T, T &, T *> iterator;
        typedef list_iterator<T, const T &, const T *> const_iterator;
        typedef typename Allocator::size_type size_type;
        typedef typename Allocator::difference_type difference_type;
        typedef typename Allocator::pointer pointer;
        typedef typename Allocator::const_pointer const_pointer;
        typedef Readable::reverse_iterator<iterator> reverse_iterator;
        typedef Readable::reverse_iterator<const_iterator> const_reverse_iterator;
    private:
        typedef list<T, Allocator> self_type;
        typedef list_node<T> node_type;
        typedef typename allocator_type::template rebind<node_type>::other node_allocator;
        list_node_base node;
    public:
        explicit list(const Allocator & = Allocator()) : node() {
            node.prev = &node;
            node.next = &node;
        }

        explicit list(size_type n, const Allocator & = Allocator()) : list() {
            for (size_t _ = 0; _ < n; ++_) {
                emplace_back();
            }
        }

        list(size_type n, const T &value, const Allocator & = Allocator()) : list() {
            for (size_t i = 0; i < n; ++i) {
                push_back(value);
            }
        }

        template<class InputIterator>
        list(InputIterator first, InputIterator last, const Allocator & = Allocator()) : list() {
            for (auto it = first; it != last; ++it) {
                push_back(*it);
            }
        }

        list(const list<T, Allocator> &x) : list() {
            for (auto &item: x) {
                push_back(x);
            }
        }

        list(list &&other) noexcept : list() {
            splice(end(), other);
        }

        list(const list &other, const Allocator &alloc) : list(other) {
        }

        list(list &&other, const Allocator &alloc) : list(other) {}

        list(std::initializer_list<T> init_list, const Allocator & = Allocator()) : list() {
            for (auto &it:init_list) {
                push_back(it);
            }
        }

        ~list() {
            clear();
        }

        list<T, Allocator> &operator=(const list<T, Allocator> &x) {
            assign(x.begin(), x.end());
            return *this;
        }

        list<T, Allocator> &operator=(list<T, Allocator> &&x) noexcept {
            node = std::move(x.node);
            return *this;
        }

        list &operator=(std::initializer_list<T> init_list) {
            assign(init_list);
            return *this;
        }

        template<typename InputIterator>
        void assign(InputIterator first, InputIterator last) {
            auto it = begin();
            while (it != end() && it != first) {
                erase(it);
            }
            if (it == first) {
                // so [first, last) is in [begin(),end)
                while (it != last) {
                    // go to last
                    ++it;
                }
                while (it != end()) {
                    // erase them all!
                    erase(it);
                }
            } else {
                // so [first, last) is not in [begin(),end)
                // just need to clear
                clear();
                insert(begin(), first, last);
            }
        }

        void assign(size_type element_count, const T &value) {
            clear();
            insert(begin(), element_count, value);
        }

        void assign(std::initializer_list<T> ilist) {
            assign(ilist.begin(), ilist.end());
        }

        allocator_type get_allocator() const noexcept {
            return allocator_type();
        }

        // iterators:
        iterator begin() noexcept {
            return iterator(node.next);
        }

        const_iterator begin() const noexcept {
            return const_iterator(node.next);
        }

        iterator end() noexcept {
            return iterator(&node);
        }

        const_iterator end() const noexcept {
            return const_iterator(&node);
        }

        reverse_iterator rbegin() noexcept {
            return reverse_iterator(end());
        }

        const_reverse_iterator rbegin() const noexcept {
            return const_reverse_iterator(end());
        }

        reverse_iterator rend() noexcept {
            return reverse_iterator(begin());
        }

        const_reverse_iterator rend() const noexcept {
            return const_reverse_iterator(begin());
        }

        const_iterator cbegin() const noexcept {
            return const_iterator(node.next);
        }

        const_iterator cend() const noexcept {
            return const_iterator(&node);
        }

        const_reverse_iterator crbegin() const noexcept {
            return const_reverse_iterator(cend());
        }

        const_reverse_iterator crend() const noexcept {
            return const_reverse_iterator(cbegin());
        }

        // capacity:
        size_type size() const noexcept {
            return distance(begin(), end());
        }

        size_type max_size() const noexcept {
            return SIZE_MAX;
        }

        void resize(size_type count) {
            resize(count, T());
        }

        void resize(size_type count, const T &value) {
            auto it = begin();
            size_t i;
            for (i = 0; i < count && it != end(); ++i, ++it) {
            }
            if (i == count) {
                // 多退
                erase(it, end());
            } else {
                // 少补
                insert(end(), count - i, value);
            }
        }

        bool empty() const noexcept {
            return node.next == &node;
        }

        // element access:
        reference front() {
            return *begin();
        }

        const_reference front() const {
            return *begin();
        }

        reference back() {
            return *(--end());
        }

        const_reference back() const {
            return *(--end());
        }

        // modifiers:
        template<class... Args>
        void emplace_front(Args &&... args) {
            emplace(iterator(begin()), std::forward<Args>(args)...);
        }

        void pop_front() {
            erase(begin());
        }

    private:
        static list_node_base *create_node(const T &value) {
            node_type *new_node = node_allocator::allocate(1);
            try {
                Readable::construct(&new_node->data, value);
            } catch (...) {
                // rollback
                node_allocator::deallocate(new_node, 1);
                throw;
            }
            return (list_node_base *) (new_node);
        }

        static list_node_base *create_node(T &&value) {
            node_type *new_node = node_allocator::allocate(1);
            try {
                Readable::construct(&new_node->data, value);
            } catch (...) {
                // rollback
                node_allocator::deallocate(new_node, 1);
                throw;
            }
            return (list_node_base *) (new_node);
        }

        static void destroy_node(node_type *node) {
            Readable::destroy(&node->data);
            node_allocator::deallocate(node, 1);
        }

    public:
        template<typename ... Args>
        void emplace_back(Args &&... args) {
            emplace(end(), std::forward<Args>(args)...);
        }

        void push_front(const T &x) {
            insert(begin(), x);
        }

        void push_front(T &&x) {
            emplace_front(std::move(x));
        }

        void push_back(const T &x) {
            insert(end(), x);
        }

        void push_back(T &&x) {
            emplace_back(std::move(x));
        }

        void pop_back() {
            erase(end());
        }

    private:
        iterator insert_node(const_iterator &position, list_node_base *new_node) const {
            auto last_node = position.node->prev;
            last_node->next = new_node;
            new_node->prev = last_node;
            new_node->next = position.node;
            position.node->prev = new_node;
            return iterator(new_node);
        }

    public:
        template<typename ... Args>
        iterator emplace(const_iterator position, Args &&... args) {
            auto new_node = create_node(T(std::forward<Args>(args)...));
            return insert_node(position, new_node);
        }

        iterator insert(const_iterator position, const T &x) {
            auto new_node = create_node(x);
            return insert_node(position, new_node);
        }

        iterator insert(const_iterator position, T &&x) {
            auto new_node = create_node(x);
            return insert_node(position, new_node);
        }

    private:
        iterator
        insert_imp(const_iterator position, size_type element_to_insert_count, const T &value, Readable::true_type) {
            size_type i = 0;
            try {
                for (; i < element_to_insert_count; ++i) {
                    insert(position, value);
                }
            } catch (...) {
                erase(next(position, -ptrdiff_t(i)), position);
            }
            return iterator(next(position, -ptrdiff_t(i)));
        }

        template<typename InputIterator>
        iterator insert_imp(const_iterator position, InputIterator first,
                            InputIterator last, Readable::false_type) {
            const_iterator now_should_insert_before(position.node);
            while (first != last) {
                now_should_insert_before = const_iterator(insert(now_should_insert_before, *first));
                ++first;
            }
            return iterator(now_should_insert_before);
        }

    public:
        iterator insert(const_iterator position, size_type n, const T &x) {
            return insert_imp(position, n, x, Readable::true_type());
        }

        template<typename InputIterator>
        iterator insert(const_iterator position, InputIterator first,
                        InputIterator last) {
            return insert_imp(position, first, last, Readable::is_integral<InputIterator>());
        }

        iterator insert(const_iterator position, std::initializer_list<T> init_list) {
            return insert(position, init_list.begin(), init_list.end());
        }

        iterator erase(const_iterator position) {
            auto prev = position.node->prev;
            auto next = position.node->next;
            prev->next = next;
            next->prev = prev;
            destroy_node((node_type *) position.node);
            return iterator(next);
        }

        iterator erase(const_iterator first, const_iterator last) {
            while (first != last)
                erase(first++);
            return last;
        }

        void swap(list<T, Allocator> &other) {
            auto temp = node;
            node = other.node;
            other.node = temp;
        }

        void clear() noexcept {
            auto cursor = node.next;
            while (cursor != &node) {
                auto tmp = cursor;
                cursor = cursor->next;
                destroy_node((node_type *) tmp);
            }
            node.next = &node;
            node.prev = &node;
        }

        // list operations:
        void splice(const_iterator position, list<T, Allocator> &other) {
            if (other == *this)
                return;
            splice(position, other, other.begin(), other.end());
        }

        void splice(const_iterator position, list<T, Allocator> &&other) {
            auto other_first = other.node.next;
            auto other_last = other.node.prev;

            position.prev.next = other_first;
            other_first->prev = position.node.prev;

            other_last->next = position.node;
            position.node->prev = other_last;
        }

        void splice(const_iterator position, list<T, Allocator> &,
                    const_iterator item) {

            auto before_item = item.node->prev;
            auto after_item = item.node->next;
            auto before_position = position.node->prev;

            before_item->next = after_item;
            after_item->prev = before_item;

            before_position->next = item.node;
            item.node->prev = before_position;
            item.node->next = position.node;
            position.node->prev = item.node;
        }

        void splice(const_iterator position, list<T, Allocator> &&,
                    const_iterator item) {
            auto before_item = item.node->prev;
            auto after_item = item.node->next;
            auto before_position = position.node->prev;
            before_item->next = after_item;
            after_item->prev = before_item;
            before_position->next = item.node;
            item.node->prev = before_position;
            item.node->next = position.node;
            position.node->prev = item.node;
        }

        void splice(const_iterator position, list<T, Allocator> &,
                    const_iterator first, const_iterator last) {
            auto before_first = first.node->prev;
            auto before_last = last.node->prev;
            auto before_position = position.node->prev;

            before_first->next = last.node;
            last.node->prev = before_first;
            before_position->next = first.node;
            first.node->prev = before_position;
            before_last->next = position.node;
            position.node->prev = before_last;
        }

        void splice(const_iterator position, list<T, Allocator> &&,
                    const_iterator first, const_iterator last) {
            auto before_first = first.node->prev;
            auto before_last = last.node->prev;
            auto before_position = position.node->prev;
            before_first->next = last.node;
            last.node->prev = before_first;
            before_position->next = first.node;
            first.node->prev = before_position;
            before_last->next = position.node;
            position.node->prev = before_last;
        }

        void remove(const T &value) {
            for (auto it = begin(); it != end();) {
                if (*it == value) {
                    it = erase(it);
                } else {
                    ++it;
                }
            }
        }

        template<class Predicate>
        void remove_if(Predicate pred) {
            for (auto it = begin(); it != end();) {
                if (pred(*it)) {
                    it = erase(it);
                } else {
                    ++it;
                }
            }
        }

        void unique() {
            for (auto it = begin(); next(it) != end(); ++it) {
                while (*it == *(next(it))) {
                    erase(next(it));
                }
            }
        }

        template<class BinaryPredicate>
        void unique(BinaryPredicate binary_pred) {
            for (auto it = begin(); next(it) != end(); ++it) {
                while (pred(*it, *(next(it)))) {
                    erase(next(it));
                }
            }
        }

        void merge(list<T, Allocator> &other) {
            merge(other, less<T>());
        }

        void merge(list<T, Allocator> &&other) {
            merge(other, less<T>());
        }

        template<class Compare>
        void merge(list<T, Allocator> &other, Compare comp) {
            auto this_it = begin();
            auto other_it = other.begin();
            while (this_it != end() && other_it != other.end()) {
                if (comp(*other_it, *this_it)) {
                    auto new_other_it = next(other_it);
                    splice(this_it, other, other_it);
                    other_it = new_other_it;
                } else {
                    ++this_it;
                }
            }
            splice(end(), other, other_it, other.end());
        }

        template<class Compare>
        void merge(list<T, Allocator> &&other, Compare comp) {
            auto this_it = begin();
            auto other_it = other.begin();
            while (this_it != end() && other_it != other.end()) {
                if (comp(*other_it, *this_it)) {
                    auto new_other_it = next(other_it);
                    splice(this_it, other, other_it);
                    other_it = new_other_it;
                } else {
                    ++this_it;
                }
            }
            splice(end(), other, other_it, other.end());
        }

    private:
        /**
         * 将链表分为两段
         * @param from 要分的部分的头
         * @param to 要分的部分的尾
         * @return 设返回的迭代器为it，则被分割的链表的第一部分是[from,it),第二部分是[it,to)
         */
        static iterator
        divide(iterator from, iterator to) {
            iterator it1 = from,
                    it2 = from;
            while (next(it2) != to && next(it2, 2) != to) {
                advance(it2, 2);
                ++it1;
            }
            return it1;
        }

        /**
         * 合并list内两个相邻的有序序列
         * @param from 第一个序列的开始
         * @param mid 第二个序列的开始，也是第一个序列的超尾
         * @param to 第二个序列的超尾
         * @param comp 比较函数
         * @return 合并后序列的开始
         */
        template<typename Compare>
        iterator merge_inside(iterator from, iterator mid, iterator to, Compare comp) {
            auto it1 = from, it2 = mid;
            while (it2 != to && it1 != it2) {
                if (!comp(*it1, *it2)) {
                    auto new_it2 = next(it2);
                    splice(it1, *this, it2);
                    if (it1 == from) {
                        from = it2;
                    }
                    it2 = new_it2;
                } else {
                    ++it1;
                }
            }
            return from;
        }

        /**
         * 将一个序列排序
         * @param from 序列的开始
         * @param to 序列的超尾
         * @param comp 比较函数
         * @return 排序后序列的开始
         */
        template<typename Compare>
        iterator sort_range(iterator from, iterator to, Compare comp) {
            if (from == to || next(from) == to) {
                return from;
            } else if (next(from, 2) == to) {
                if (!comp(*from, *to)) {
                    splice(from, *this, to);
                    return to;
                }
                return from;
            }
            auto mid = divide(from, to);
            auto new_from = sort_range(from, mid, comp);
            mid = sort_range(mid, to, comp);
            return merge_inside(new_from, mid, to, comp);
        }

    public:
        void sort() {
            sort(less<T>());
        }

        template<class Compare>
        void sort(Compare comp) {
            sort_range(begin(), end(), comp);
        }

        void reverse() noexcept {
            auto this_node = &node;
            auto next_node = node.next;
            while (next_node != &node) {
                next_node = this_node->next;
                this_node->next = this_node->prev;
                this_node->prev = next_node;
            }
        }
    };
}
#endif //STL_FROM_SCRATCH_LIST_H
