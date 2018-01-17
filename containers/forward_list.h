#ifndef STL_FROM_SCRATCH_FORWARD_LIST_H
#define STL_FROM_SCRATCH_FORWARD_LIST_H

#include "../memory/allocator.h"
#include "../iterator/iterator_traits.h"
#include "../memory/uninitialized_memory_functions.h"
#include "../utility/utility.h"
#include "../type_traits.h"

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
    public:
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef ReferenceType reference;
        typedef PointerType pointer;
        typedef Readable::forward_iterator_tag iterator_category;
        typedef forward_list_iterator<T, ReferenceType, PointerType> self_type;
        typedef forward_list_iterator<T, T &, T *> iterator;
        typedef forward_list_iterator<T, const T &, const T *> const_iterator;
        forward_list_node_base *node;

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

        // 下面这两个函数由于模版匹配的优先级高于整型隐式提升，故在传入两个int时匹配存在问题
        // 故需借由traits进行转发
        explicit forward_list(size_type element_count,
                              const T &value = T(),
                              const Allocator &alloc = Allocator()) : forward_list() {
            constructor(element_count, value, std::true_type());
        }

        template<typename InputIt>
        forward_list(InputIt first, InputIt last,
                     const Allocator &alloc = Allocator()):forward_list(alloc) {
            // 若is_integral为true_type，则将first判为iterator则为错判，改为调用正确的constructor
            // 否则判断正确，调用相应的constructor
            constructor(first, last, std::is_integral<InputIt>());
        }

        template<typename Integral>
        void constructor(Integral element_count,
                         const T &value, std::true_type) {
            insert_after(before_begin(), (size_type) element_count, value);
        }

        template<typename InputIt>
        void constructor(InputIt first, InputIt last,
                         std::false_type) {
            insert_after(before_begin(), first, last);
        }


        forward_list(const forward_list<T, Allocator> &other, const Allocator &alloc = Allocator()) : forward_list(
                alloc) {
            insert_after(before_begin(), other.begin(), other.end());
        }

        forward_list(forward_list<T, Allocator> &&other, const Allocator &alloc = Allocator()) : node_before_begin(
                std::move(other.node_before_begin)) {}


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

        size_type max_size() const noexcept {
            return SIZE_MAX;
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

        template<typename InputIt>
        iterator insert_after(const_iterator pos, InputIt first, InputIt last) {
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

        void push_front(const T &value) {
            insert_after(before_begin(), value);
        }

        void push_front(T &&value) {
            insert_after(before_begin(), value);
        }

        void pop_front() {
            erase_after(before_begin());
        }

        void resize(size_type count) {
            resize(count, T());
        }

        void resize(size_type count, const value_type &value) {
            auto it = before_begin();
            for (size_t i = 0; i < count; ++i, ++it) {
                if (empty() || !(it.node->next)) {
                    insert_after(it, value);
                }
            }
            while (it != before_begin() && it != end() && it.node->next) {
                erase_after(it);
            }
        }

        template<typename alloc>
        void swap(forward_list<T, alloc> &other) {
            std::swap(node_before_begin.next, other.node_before_begin.next);
        }

        template<typename alloc>
        void merge(forward_list<T, alloc> &other) {
            auto this_now = before_begin();
            auto this_next = begin();
            auto other_now = other.before_begin();
            auto other_next = other.begin();
            while (this_next != end() && other_next != end()) {
                if (*this_next < *other_next) {
                    ++this_next;
                    ++this_now;
                } else {
                    insert_after(this_now, *other_next);
                    this_next = this_now;
                    ++this_next;
                    ++other_now;
                    ++other_next;
                }
            }
            while (other_next != end()) {
                insert_after(this_now, *other_next);
                ++this_now;
                ++other_now;
                ++other_next;
            }
            other.clear();
        }

        template<typename alloc>
        void merge(forward_list<T, alloc> &&other) {
            auto this_now = before_begin();
            auto this_next = begin();
            auto other_now = other.before_begin();
            auto other_next = other.begin();
            while (this_next != end() && other_next != end()) {
                if (*this_next < *other_next) {
                    ++this_next;
                    ++this_now;
                } else {
                    insert_after(this_now, *other_next);
                    this_next = this_now;
                    ++this_next;
                    ++other_now;
                    ++other_next;
                }
            }
            while (other_next != end()) {
                insert_after(this_now, *other_next);
                ++this_now;
                ++other_now;
                ++other_next;
            }
        }


        template<typename alloc, typename Compare>
        void merge(forward_list<T, alloc> &other, Compare comp) {
            auto this_now = before_begin();
            auto this_next = begin();
            auto other_now = other.before_begin();
            auto other_next = other.begin();
            while (this_next != end() && other_next != end()) {
                if (comp(*this_next, *other_next)) {
                    ++this_next;
                    ++this_now;
                } else {
                    insert_after(this_now, *other_next);
                    this_next = this_now;
                    ++this_next;
                    ++other_now;
                    ++other_next;
                }
            }
            while (other_next != end()) {
                insert_after(this_now, *other_next);
                ++this_now;
                ++other_now;
                ++other_next;
            }
            other.clear();
        };

        template<typename alloc, typename Compare>
        void merge(forward_list<T, alloc> &&other, Compare comp) {
            auto this_now = before_begin();
            auto this_next = begin();
            auto other_now = other.before_begin();
            auto other_next = other.begin();
            while (this_next != end() && other_next != end()) {
                if (comp(*this_next, *other_next)) {
                    ++this_next;
                    ++this_now;
                } else {
                    insert_after(this_now, *other_next);
                    this_next = this_now;
                    ++this_next;
                    ++other_now;
                    ++other_next;
                }
            }
            while (other_next != end()) {
                insert_after(this_now, *other_next);
                ++this_now;
                ++other_now;
                ++other_next;
            }
        }

        template<typename alloc>
        void splice_after(const_iterator pos, forward_list<T, alloc> &other) {
            splice_after(pos, other, other.before_begin(), other.end());
        }

        template<typename alloc>
        void splice_after(const_iterator pos, forward_list<T, alloc> &&other) {
            splice_after(pos, other, other.before_begin(), other.end());
        }

        template<typename alloc>
        void splice_after(const_iterator pos, forward_list<T, alloc> &other,
                          const_iterator it) {
            splice_after(pos, other, it, other.end());
        }

        template<typename alloc>
        void splice_after(const_iterator pos, forward_list<T, alloc> &&other,
                          const_iterator it) {
            splice_after(pos, other, it, other.end());
        }

        template<typename alloc>
        void splice_after(const_iterator pos, forward_list<T, alloc> &other,
                          const_iterator first, const_iterator last) {
            if (first != last && pos != first) {
                const_iterator it = first;
                while (it.node->next != last.node)
                    ++it;
                if (first != it) {
                    it.node->next = pos.node->next;
                    pos.node->next = first.node->next;
                    first.node->next = last.node;
                }
            }
        }

        template<typename alloc>
        void splice_after(const_iterator pos, forward_list<T, alloc> &&other,
                          const_iterator first, const_iterator last) {
            if (first != last && pos != first) {
                const_iterator it = first;
                while (it.node->next != last.node)
                    ++it;
                if (first != it) {
                    it.node->next = pos.node->next;
                    pos.node->next = first.node->next;
                    first.node->next = last.node;
                }
            }
        }

        void remove(const T &value) {
            for (auto it = before_begin(); next(it) != end();) {
                if (*next(it) == value) {
                    erase_after(it);
                } else {
                    ++it;
                }
            }
        }

        template<typename UnaryPredicate>
        void remove_if(UnaryPredicate p) {
            for (auto it = before_begin(); next(it) != end();) {
                if (p(*next(it))) {
                    erase_after(it);
                } else {
                    ++it;
                }
            }
        }

        void reverse() noexcept {
            if (begin() != end() && next(begin()) != end()) {
                auto new_last = begin().node;
                for (auto it = begin(); next(it) != end(); ++it) {
                    auto the_node_we_are_dealing_with = it.node.next;
                    auto old_begin_node = node_before_begin.next;
                    new_last->next = the_node_we_are_dealing_with->next;
                    the_node_we_are_dealing_with->next = old_begin_node;
                    node_before_begin.next = the_node_we_are_dealing_with;
                }
            }
        }

        void unique() {
            for (auto it = begin(); next(it) != end();) {
                if (*next(it) == *it) {
                    erase_after(it);
                } else {
                    ++it;
                }
            }
        }

        template<typename BinaryPredicate>
        void unique(BinaryPredicate p) {
            for (auto it = begin(); next(it) != end(); ++it) {
                if (p(*next(it), *it)) {
                    erase_after(it);
                }
            }
        }

        void sort() {
            sort([](const T &a, const T &b) { return a < b; });
        }

    private:
        static Readable::pair<forward_list_node_base *, forward_list_node_base *>
        divide(forward_list_node_base *first) {
            forward_list_node_base *tail1 = first;
            forward_list_node_base *tail2 = first;
            while (tail2->next && tail2->next->next) {
                tail1 = tail1->next;
                tail2 = tail2->next->next;
            }
            auto head2 = tail1->next;
            tail1->next = nullptr;
            return Readable::pair<forward_list_node_base *, forward_list_node_base *>(first, head2);
        }

        template<typename Compare>
        static forward_list_node_base *merge(forward_list_node_base *l1, forward_list_node_base *l2, Compare comp) {
            if (!l1) {
                return l2;
            } else if (!l2) {
                return l1;
            }
            auto first_now_at = l1;
            auto second_now_at = l2;
            decltype(first_now_at) ret = nullptr;
            if (comp(((forward_list_node<T> *) first_now_at)->value, ((forward_list_node<T> *) second_now_at)->value)) {
                ret = first_now_at;
                first_now_at = first_now_at->next;
                ret->next = nullptr;
            } else {
                ret = second_now_at;
                second_now_at = second_now_at->next;
                ret->next = nullptr;
            }
            auto ret_head = ret;
            while (first_now_at && second_now_at) {
                if (comp(((forward_list_node<T> *) first_now_at)->value,
                         ((forward_list_node<T> *) second_now_at)->value)) {
                    ret->next = first_now_at;
                    first_now_at = first_now_at->next;
                    ret = ret->next;
                    ret->next = nullptr;
                } else {
                    ret->next = second_now_at;
                    second_now_at = second_now_at->next;
                    ret = ret->next;
                    ret->next = nullptr;
                }
            }
            if (first_now_at) {
                ret->next = first_now_at;
            }
            if (second_now_at) {
                ret->next = second_now_at;
            }
            return ret_head;
        }

        template<typename Compare>
        static forward_list_node_base *_sort(forward_list_node_base *list, Compare comp) {
            if (!list || !(list->next)) {
                return list;
            }
            Readable::pair<forward_list_node_base *, forward_list_node_base *> divided = divide(list);
            return merge(_sort(divided.first, comp), _sort(divided.second, comp), comp);
        }

    public:
        template<typename Compare>
        void sort(Compare comp) {
            node_before_begin.next = _sort(forward_list<T>::node_before_begin.next, comp);
        }
    };
}
#endif //STL_FROM_SCRATCH_FORWARD_LIST_H
