#ifndef STL_FROM_SCRATCH_FORWARD_LIST_H
#define STL_FROM_SCRATCH_FORWARD_LIST_H

#include "../memory/memory.h"
#include "../iterator/iterator_traits.h"
#include "../utility/utility.h"
#include "../type_traits/type_traits.h"

namespace Readable {
    /**
     * forward_list的节点基类
     * 储存了与forward_list保存元素类型无关的内容
     * 即指向下一个节点的指针
     */
    struct forward_list_node_base {
        forward_list_node_base *next;
    };

    /**
     * forward_list的节点
     * @tparam T forward_list保存的元素类型
     */
    template<typename T>
    struct forward_list_node : forward_list_node_base {
        T value;

        /**
         * 用 @arg val 构造forward_list的节点
         * @param val 要保存的值
         */
        forward_list_node(const T &val) : value(val) {}

        forward_list_node(T &&val) : value(std::move(val)) {}
    };

    /**
     * forward_list的迭代器
     * @tparam ReferenceType 引用类型，可以为const或非const
     * @tparam PointerType 指针类型，可以为const或非const
     */
    template<typename T, typename ReferenceType, typename PointerType>
    class forward_list_iterator {
    public:
        // 以下是iterator_traits所需知道的内容
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef ReferenceType reference;
        typedef PointerType pointer;
        typedef Readable::forward_iterator_tag iterator_category;
        // 为方便起见定义的一些类型
        // 本身的类型
        typedef forward_list_iterator<T, ReferenceType, PointerType> self_type;
        // 正常的迭代器类型
        typedef forward_list_iterator<T, T &, T *> iterator;
        // const的迭代器类型
        typedef forward_list_iterator<T, const T &, const T *> const_iterator;
        // 实际保存的内容
        forward_list_node_base *node;

        // 构造函数
        explicit forward_list_iterator(forward_list_node_base *the_node = nullptr) : node(the_node) {}

        // 使iterator可以拷贝构造自无论是否const的iterator
        explicit forward_list_iterator(const iterator &other) : node(other.node) {}

        explicit forward_list_iterator(const const_iterator &other) : node(other.node) {}

        // 下面实现标准文件规定forward_iterator应该实现的所有操作
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

    /**
     * forward_list容器类
     * @tparam T 容器中的内容
     * @tparam Allocator 空间分配器
     */
    template<typename T, typename Allocator = Readable::allocator<T>>
    class forward_list {
    public:
        typedef T value_type;
        typedef Allocator allocator_type;

        static_assert((Readable::is_same<typename allocator_type::value_type, value_type>::value),
                      "Allocator::value_type must be same type as value_type");

        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef value_type &reference;
        typedef const value_type &const_reference;
        typedef typename std::allocator_traits<Allocator>::pointer pointer;
        typedef typename std::allocator_traits<Allocator>::const_pointer const_pointer;
        typedef forward_list_iterator<T, T &, T *> iterator;
        typedef forward_list_iterator<T, const T &, const T *> const_iterator;
    private:
        typedef forward_list<T, Allocator> self_type;
        typedef forward_list_node<T> node_type;
        typedef typename allocator_type::template rebind<node_type>::other node_allocator;
        forward_list_node_base node_before_begin;
    public:
        // 使用Allocator类型参数构造forward_list时，实际上并不使用这个参数
        explicit forward_list(const Allocator &) {
            node_before_begin.next = nullptr;
        }

        forward_list() : forward_list(Allocator()) {}

        // 下面这两个函数由于模版匹配的优先级高于整型隐式提升
        // 如果使用最naive的实现，在传入两个int时匹配存在问题
        // 故需借由traits进行dispatch
        // 实际dispatch的操作在insert_after中
        explicit forward_list(size_type element_count,
                              const T &value = T(),
                              const Allocator &alloc = Allocator()) : forward_list() {
            insert_after(before_begin(), element_count, value);
        }

        template<typename InputIt>
        forward_list(InputIt first, InputIt last,
                     const Allocator &alloc = Allocator()):forward_list(alloc) {
            insert_after(before_begin(), first, last);
        }


        // 为了兼容使用不同空间配置器的forward_list，我们将alloc_type作为模版参数
        template<typename alloc_type>
        forward_list(const forward_list<T, alloc_type> &other, const Allocator &alloc = Allocator()) : forward_list(
                alloc) {
            insert_after(before_begin(), other.begin(), other.end());
        }

        // 对于move_constructor，只在copy_counstruct的other和自己类型完全相同时才能使用move加速
        // 对于other和自己类型不完全相同的情况，other将不被看作将亡值，而由上面一个函数进行逐元素处理
        forward_list(self_type &&other, const Allocator &alloc = Allocator()) : node_before_begin(
                std::move(other.node_before_begin)) {}

        forward_list(std::initializer_list<T> init,
                     const Allocator &alloc = Allocator()) : forward_list(init.begin(), init.end(), alloc) {}

        ~forward_list() { clear(); }

        template<typename alloc_type>
        self_type &operator=(const forward_list<T, alloc_type> &other) {
            assign(other.begin(), other.end());
            return *this;
        }

        template<typename alloc_type>
        self_type &operator=(forward_list<T, alloc_type> &&other) noexcept {
            node_before_begin = std::move(other.node_before_begin);
            return *this;
        }

        self_type &operator=(std::initializer_list<T> ilist) {
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
            node_type *new_node = node_allocator::allocate(1);
            try {
                construct(&new_node->value, value);
            } catch (...) {
                // rollback
                node_allocator::deallocate(new_node, 1);
                throw;
            }
            return (forward_list_node_base *) (new_node);
        }

        static forward_list_node_base *create_node(T &&value) {
            node_type *new_node = node_allocator::allocate(1);
            try {
                construct(&new_node->value, value);
            } catch (...) {
                // rollback
                node_allocator::deallocate(new_node, 1);
                throw;
            }
            return (forward_list_node_base *) (new_node);
        }

        static void destory_node(node_type *node) {
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

    private:
        // 下面两个函数是insert_after的实际实现
        iterator
        insert_after_imp(const_iterator pos, size_type element_to_insert_count, const T &value, Readable::true_type) {
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
            return iterator(pos);
        }

        template<typename InputIt>
        iterator insert_after_imp(const_iterator pos, InputIt first, InputIt last, Readable::false_type) {
            const_iterator now_should_insert_after(pos.node);
            while (first != last) {
                now_should_insert_after = const_iterator(insert_after(now_should_insert_after, *first));
                ++first;
            }
            return iterator(now_should_insert_after);
        }

    public:
        iterator
        insert_after(const_iterator pos, size_type element_to_insert_count, const T &value) {
            return insert_after_imp(pos, element_to_insert_count, value, Readable::true_type());
        }

        template<typename InputIt>
        iterator insert_after(const_iterator pos, InputIt first_param, InputIt second_param) {
            // 若"InputIt"为整数类型，则将其判断为迭代器为错判
            // 此时is_integral为true_type，会将其重新匹配到正确的实现
            return insert_after_imp(pos, first_param, second_param, Readable::is_integral<InputIt>());
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
            forward_list_node_base *the_node_to_erase_after = pos.node;
            if (the_node_to_erase_after->next) {
                auto next_of_next = the_node_to_erase_after->next->next;
                destory_node((node_type *) the_node_to_erase_after->next);
                the_node_to_erase_after->next = next_of_next;
            }
            return iterator(the_node_to_erase_after->next);
        }

        iterator erase_after(const_iterator first, const_iterator last) {
            forward_list_node_base *node_first = first.node;
            forward_list_node_base *node_last = last.node;
            if (node_first != node_last) {
                while (node_first->next != node_last) {
                    auto next_of_next = node_first->next->next;
                    destory_node((node_type *) node_first->next);
                    node_first->next = next_of_next;
                }
            }
            return iterator(last);
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

        // todo: 实现这个
        //template< class... Args >
        //void emplace_front( Args&&... args );

        void pop_front() {
            erase_after(before_begin());
        }

        void resize(size_type count) {
            resize(count, T());
        }

        void resize(size_type count, const value_type &value) {
            auto it = before_begin();
            for (size_t i = 0; i < count; ++i, ++it) {
                // 少补
                if (empty() || !(it.node->next)) {
                    insert_after(it, value);
                }
            }
            // 多退
            while (it != before_begin() && it != end() && it.node->next) {
                erase_after(it);
            }
        }

        template<typename alloc>
        void swap(forward_list<T, alloc> &other) {
            std::swap(node_before_begin.next, other.node_before_begin.next);
        }

        // merge族函数还存在优化空间——考虑将insert_after改为splice_after的实现
        template<typename alloc>
        void merge(forward_list<T, alloc> &other) {
            // 储存this中现在处理到的位置和其next
            auto this_now = before_begin();
            auto this_next = begin();
            // 储存other中现在处理到的位置和其next
            auto other_now = other.before_begin();
            auto other_next = other.begin();
            while (this_next != end() && other_next != end()) {
                if (*this_next < *other_next) {
                    // 直接处理this中下一个元素
                    ++this_next;
                    ++this_now;
                } else { // *this_next >= *other_next
                    // 将other_next并入this，插入到this_now之后，this_next之前
                    insert_after(this_now, *other_next);
                    // 这两句将this_next置为刚刚插入的节点
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
            // other为将亡值，无需clear
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
            if (pos != it && next(pos) != it)
                splice_after(pos, other, it, next(it, 2));
        }

        template<typename alloc>
        void splice_after(const_iterator pos, forward_list<T, alloc> &&other,
                          const_iterator it) {
            if (pos != it && next(pos) != it)
                splice_after(pos, other, it, next(it, 2));
        }

        template<typename alloc>
        void splice_after(const_iterator pos, forward_list<T, alloc> &other,
                          const_iterator first, const_iterator last) {
            if (first != last && pos != first) {
                // 找到last前的节点（对应的迭代器）
                const_iterator the_iterator_before_last = first;
                while (the_iterator_before_last.node->next != last.node)
                    ++the_iterator_before_last;
                // 要进入this的一段是(first,the_iterator_before_last]
                if (first != the_iterator_before_last) {
                    // 先将the_iterator_before_last加入链中
                    the_iterator_before_last.node->next = pos.node->next;
                    // 将first之后的节点加入链中
                    pos.node->next = first.node->next;
                    // 从原链中移除这一段
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
            // 三指针反转法
            // hard to explain, may be bad code
            // but it just works
            // todo: 尝试解释这个
            if (begin() != end() && next(begin()) != end()) {
                auto last_node_in_reversed_list = begin().node;
                for (auto it = begin(); next(it) != end(); ++it) {
                    auto the_node_we_are_dealing_with = it.node.next;
                    auto old_begin_node = node_before_begin.next;
                    last_node_in_reversed_list->next = the_node_we_are_dealing_with->next;
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
        // 将链表均分为两段
        static Readable::pair<forward_list_node_base *, forward_list_node_base *>
        divide(forward_list_node_base *first) {
            forward_list_node_base *tail1 = first;
            forward_list_node_base *tail2 = first;
            // 龟兔算法
            while (tail2->next && tail2->next->next) {
                tail1 = tail1->next;
                tail2 = tail2->next->next;
            }
            auto head2 = tail1->next;
            // 断开链表
            tail1->next = nullptr;
            return Readable::pair<forward_list_node_base *, forward_list_node_base *>(first, head2);
        }

        // 基于node的归并
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
            // 取出两个要归并的链表头中在先的一个，放入ret中
            if (comp(((node_type *) first_now_at)->value, ((node_type *) second_now_at)->value)) {
                ret = first_now_at;
                first_now_at = first_now_at->next;
                ret->next = nullptr;
            } else {
                ret = second_now_at;
                second_now_at = second_now_at->next;
                ret->next = nullptr;
            }
            // 下面与基于迭代器的merge相似
            auto ret_head = ret;
            while (first_now_at && second_now_at) {
                if (comp(((node_type *) first_now_at)->value,
                         ((node_type *) second_now_at)->value)) {
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

        // 基于node的排序
        template<typename Compare>
        static forward_list_node_base *sort_by_node(forward_list_node_base *list, Compare comp) {
            if (!list || !(list->next)) {
                return list;
            }
            Readable::pair<forward_list_node_base *, forward_list_node_base *> divided = divide(list);
            return merge(sort_by_node(divided.first, comp), sort_by_node(divided.second, comp), comp);
        }

    public:
        template<typename Compare>
        void sort(Compare comp) {
            node_before_begin.next = sort_by_node(forward_list<T, allocator_type>::node_before_begin.next, comp);
        }
    };
}
#endif //STL_FROM_SCRATCH_FORWARD_LIST_H
