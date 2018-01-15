#ifndef STL_FROM_SCRATCH_FORWARD_LIST_H
#define STL_FROM_SCRATCH_FORWARD_LIST_H

#include "../memory/allocator.h"
#include "../iterator/iterator_traits.h"
#include "../memory/uninitialized_memory_functions.h"

namespace Readable {
    struct forward_list_node_base {
        forward_list_node_base *next;
    };

    template<typename T>
    struct forward_list_node : forward_list_node_base {
        T value;

        forward_list_node(const T &val) : value(val) {}

        forward_list_node(T &&val) : value(std::move(val)) {}
    };

    template<typename T, typename Allocator = Readable::allocator<T>>
    class forward_list;

    template<typename T, typename ReferenceType, typename PointerType>
    class forward_list_iterator {
        template<typename T_, typename Allocator>
        friend
        class forward_list;

    public:
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef ReferenceType reference;
        typedef PointerType pointer;
        typedef Readable::forward_iterator_tag iterator_category;
        typedef forward_list_iterator<T, ReferenceType, PointerType> self_type;
    protected:
        typedef forward_list_iterator<T, T &, T *> iterator;
        typedef forward_list_iterator<T, const T &, const T *> const_iterator;
    public:
        forward_list_node_base *node;
    public:
        forward_list_iterator(forward_list_node_base *the_node = nullptr) : node(the_node) {}

        forward_list_iterator(const iterator &other) : node(other.node) {}

        forward_list_iterator(const const_iterator &other) : node(other.node) {}

        bool operator==(const forward_list_iterator &rhs) const {
            return node == rhs.node;
        }

        bool operator!=(const forward_list_iterator &rhs) const {
            return !(rhs == *this);
        }

        T &operator*() const {
            return ((forward_list_node<T> *) (node))->value;
        }

        T *operator->() const {
            return &(operator*());
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
    };

    template<typename T, typename Allocator>
    class forward_list {
    public:
        typedef T value_type;
        typedef Allocator allocator_type;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef value_type &reference;
        typedef const value_type &const_reference;
        typedef typename std::allocator_traits<Allocator>::pointer pointer;
        typedef typename std::allocator_traits<Allocator>::const_pointer const_pointer;
        typedef forward_list_iterator<T, T &, T *> iterator;
        typedef forward_list_iterator<T, const T &, const T *> const_iterator;
    private:
        typedef Readable::allocator<forward_list_node<T>> node_allocator;
        forward_list_node_base node_before_begin;
    public:
        // 使用Allocator类型参数构造forward_list时，实际上并不使用这个参数
        explicit forward_list(const Allocator &) {
            node_before_begin.next = nullptr;
        }

        forward_list() : forward_list(Allocator()) {}

        explicit forward_list(size_type element_count,
                              const T &value = T(),
                              const Allocator &alloc = Allocator()) : forward_list() {
            insert_after(before_begin(), element_count, value);
        }

        forward_list(const forward_list<T, Allocator> &other, const Allocator &alloc = Allocator()) : forward_list(
                alloc) {
            insert_after(before_begin(), other.begin(), other.end());
        }

        forward_list(forward_list<T, Allocator> &&other, const Allocator &alloc = Allocator()) : node_before_begin(
                std::move(other.node_before_begin)) {}

        template<class InputIt>
        forward_list(InputIt first, InputIt last,
                     const Allocator &alloc = Allocator()):forward_list(alloc) {
            insert_after(before_begin(), first, last);
        }

        forward_list(std::initializer_list<T> init,
                     const Allocator &alloc = Allocator()) : forward_list(init.begin(), init.end(), alloc) {}

        ~forward_list() { clear(); }

        forward_list<T, Allocator> &operator=(const forward_list<T, Allocator> &other) {
            assign(other.begin(), other.end());
            return *this;
        }

        forward_list &operator=(forward_list &&other) noexcept {
            node_before_begin = std::move(other.node_before_begin);
            return *this;
        }

        forward_list &operator=(std::initializer_list<T> ilist) {
            assign(ilist);
            return *this;
        }

        void assign(size_type element_count, const T &value) {
            clear();
            insert_after(before_begin(), element_count, value);
        }

        template<class InputIt>
        void assign(InputIt first, InputIt last) {
            clear();
            insert_after(before_begin(), first, last);
        }

        void assign(std::initializer_list<T> ilist) {
            assign(ilist.begin(), ilist.end());
        }

        allocator_type get_allocator() const {
            return allocator_type();
        }

        reference front() {
            return *(begin());
        }

        const_reference front() const {
            return *(begin());
        }

        iterator before_begin() noexcept {
            return iterator(&node_before_begin);
        }

        const_iterator before_begin() const noexcept {
            return const_iterator(&node_before_begin);
        }

        const_iterator cbefore_begin() const noexcept {
            return const_iterator(&node_before_begin);
        }

        iterator begin() noexcept {
            return iterator(node_before_begin.next);
        }

        const_iterator begin() const noexcept {
            return const_iterator(node_before_begin.next);
        }

        const_iterator cbegin() const noexcept {
            return const_iterator(node_before_begin.next);
        }

        iterator end() noexcept {
            return iterator(nullptr);
        }

        const_iterator end() const noexcept {
            return const_iterator(nullptr);
        }

        const_iterator cend() const noexcept {
            return const_iterator(nullptr);
        }

        bool empty() const noexcept {
            return node_before_begin.next == nullptr;
        }

    private:
        static forward_list_node_base *create_node(const T &value) {
            forward_list_node<T> *new_node = node_allocator::allocate(1);
            try {
                construct(&new_node->value, value);
            } catch (...) {
                node_allocator::deallocate(new_node, 1);
                throw;
            }
            return (forward_list_node_base *) (new_node);
        }

        static forward_list_node_base *create_node(T &&value) {
            forward_list_node<T> *new_node = node_allocator::allocate(1);
            try {
                construct(&new_node->value, value);
            } catch (...) {
                node_allocator::deallocate(new_node, 1);
                throw;
            }
            return (forward_list_node_base *) (new_node);
        }

        static void destory_node(forward_list_node<T> *node) {
            destroy(&node->value);
            node_allocator::deallocate(node, 1);
        }

        forward_list_node_base *
        insert_after(forward_list_node_base *node_to_be_inserted_after, forward_list_node_base *node_to_insert) const {
            node_to_insert->next = node_to_be_inserted_after->next;
            node_to_be_inserted_after->next = node_to_insert;
            return node_to_insert;
        }

    public:
        iterator insert_after(const_iterator pos, const T &value) {
            forward_list_node_base *node_to_be_inserted_after = pos.node;
            forward_list_node_base *node_to_insert = create_node(value);
            auto inserted_node = insert_after(node_to_be_inserted_after, node_to_insert);
            return iterator(inserted_node);
        }

        iterator insert_after(const_iterator pos, T &&value) {
            forward_list_node_base *node_to_be_inserted_after = pos.node;
            forward_list_node_base *node_to_insert = create_node(value);
            return iterator(insert_after(node_to_be_inserted_after, node_to_insert));
        }

        iterator
        insert_after(const_iterator pos, size_type element_to_insert_count, const T &value) {
            size_type i = 0;
            const_iterator origin_pos(pos);
            try {
                for (; i < element_to_insert_count; ++i) {
                    insert_after(pos, value);
                    ++pos;
                }
            } catch (...) {
                for (size_type j = 0; j < i; ++j) {
                    erase_after(origin_pos);
                }
            }
            return pos;
        }

        template<class InputIt>
        typename std::enable_if<std::__is_input_iterator<InputIt>::value, typename forward_list<T, Allocator>::iterator>::type
        insert_after(const_iterator pos, InputIt first, InputIt last) {
            const_iterator now_should_insert_after(pos.node);
            while (first != last) {
                now_should_insert_after = insert_after(now_should_insert_after, *first);
                ++first;
            }
            return now_should_insert_after;
        }


        iterator insert_after(const_iterator pos, std::initializer_list<T> ilist) {
            return insert_after(pos, ilist.begin(), ilist.end());
        }

        // todo: 这个实现需要修改
//        template<class... Args>
//        iterator emplace_after(const_iterator pos, Args &&... args) {
//            forward_list_node_base *node_to_be_inserted_after = pos.node;
//            forward_list_node_base *node_to_insert = create_node(T(args));
//            return iterator(insert_after(node_to_be_inserted_after, node_to_insert));
//        }

        iterator erase_after(const_iterator pos) {
            forward_list_node_base *the_node_to_insert_after = pos.node;
            if (the_node_to_insert_after->next) {
                auto next_next = the_node_to_insert_after->next->next;
                destory_node((forward_list_node<T> *) the_node_to_insert_after->next);
                the_node_to_insert_after->next = next_next;
            }
            return iterator(the_node_to_insert_after->next);
        }

        iterator erase_after(const_iterator first, const_iterator last) {
            forward_list_node_base *node_first = first.node;
            forward_list_node_base *node_last = last.node;
            if (node_first != node_last) {
                while (node_first->next != node_last) {
                    auto next_next = node_first->next->next;
                    destory_node((forward_list_node<T> *) node_first->next);
                    node_first->next = next_next;
                }
            }
            return last;
        }

        void clear() {
            erase_after(before_begin(), end());
        }
    };
}
#endif //STL_FROM_SCRATCH_FORWARD_LIST_H
