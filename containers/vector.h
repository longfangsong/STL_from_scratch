//
// Created by 龙方淞 on 2018/1/22.
//

#ifndef STL_FROM_SCRATCH_VECTOR_H
#define STL_FROM_SCRATCH_VECTOR_H

#include "../memory/allocator.h"
#include "../iterator/iterator.h"
#include "../type_traits/type_traits.h"
#include "../algorithm/algorithm.h"
#include "../memory/uninitialized_memory_functions.h"

namespace Readable {
    template<typename T, typename Allocator = Readable::allocator<T> >
    class vector final {
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
        typedef pointer iterator;
        typedef const_pointer const_iterator;
        typedef Readable::reverse_iterator<iterator> reverse_iterator;
        typedef Readable::reverse_iterator<const_iterator> const_reverse_iterator;

    private:
        pointer start;
        pointer finish;
        pointer end_of_storage;
    public:
        explicit vector(const Allocator &alloc = Allocator()) : start(nullptr), finish(nullptr),
                                                                end_of_storage(nullptr) {};

        explicit vector(size_type count) {
            start = allocator_type::allocate(count);
            finish = start;
            end_of_storage = finish + count;
        }

    private:
        template<typename InputIt>
        void initialize(InputIt first, InputIt last, Readable::false_type) {
            auto n = static_cast<size_type>(distance(first, last));
            start = allocator_type::allocate(n);
            end_of_storage = start + n;
            finish = Readable::uninitialized_copy(first, last, start);
        }


        template<class Integer>
        void initialize(Integer n, Integer value, Readable::true_type) {
            start = allocator_type::allocate(n);
            end_of_storage = start + n;
            finish = uninitialized_fill_n(start, n, value);
        }

    public:
        vector(size_type count,
               const T &value,
               const Allocator &alloc = Allocator()) {
            initialize(count, value, Readable::true_type());
        }

        template<typename InputItOrIntegral>
        vector(InputItOrIntegral first, InputItOrIntegral last,
               const Allocator &alloc = Allocator()) {
            initialize(first, last, Readable::is_integral<InputItOrIntegral>());
        }

        template<typename alloc_type>
        vector(const vector<T, alloc_type> &other):vector(other.begin(), other.end()) {}

        template<typename alloc_type>
        vector(const vector<T, alloc_type> &other, const Allocator &alloc) : vector(other) {};

        vector(vector &&other) :
                start(std::move(other.start)),
                finish(std::move(other.finish)),
                end_of_storage(std::move(other.end_of_storage)) {}

        vector(vector &&other, const Allocator &alloc) : vector(other) {}

        vector(const std::initializer_list<T> &init,
               const Allocator &alloc = Allocator()) : vector(init.begin(), init.end()) {}

        ~vector() {
            clear();
            allocator_type::deallocate(start, end_of_storage - start);
        }

    private:
        void expand_space_to(size_type capacity_want) {
            if (capacity_want > capacity()) {
                auto new_start = allocator_type::allocate(capacity_want);
                finish = Readable::uninitialized_move(begin(), end(), new_start);
                allocator_type::deallocate(start, end_of_storage - start);
                start = new_start;
                end_of_storage = start + capacity_want;
            }
        }

        void assign(size_type count, const T &value, Readable::true_type) {
            clear();
            reserve(count);
            finish = uninitialized_fill_n(start, count, value);
        }

        template<typename InputIt>
        void assign(InputIt first, InputIt last, Readable::false_type) {
            if (start <= first && last <= finish) {
                // [first,last) is in [start,finish)
                // so we are dealing a range within the vector itself
                erase(start, first);
                erase(last, finish);
            } else {
                clear();
                reserve(static_cast<size_type>(Readable::distance(first, last)));
                finish = uninitialized_copy(first, last, start);
            }
        }

    public:
        void assign(size_type count, const T &value) {
            assign(count, value, Readable::true_type());
        }

        template<typename InputItOrIntegral>
        void assign(InputItOrIntegral first_param, InputItOrIntegral second_param) {
            assign(first_param, second_param, Readable::is_integral<InputItOrIntegral>());
        }

        void assign(const std::initializer_list<T> &ilist) {
            clear();
            reserve(ilist.size());
            finish = uninitialized_move(ilist.begin(), ilist.end(), start);
        }

        template<typename alloc_type>
        vector &operator=(const vector<T, alloc_type> &other) {
            // safe when self assignment
            // see the source code of assign
            assign(other.begin(), other.end());
            return *this;
        }

        vector &operator=(const vector &other) {
            assign(other.begin(), other.end());
            return *this;
        }

        vector &operator=(vector &&other) {
            clear();
            start = std::move(other.start);
            end_of_storage = std::move(other.end_of_storage);
            finish = std::move(other.finish);
            return *this;
        }

        vector &operator=(const std::initializer_list<T> &ilist) {
            assign(ilist);
            return *this;
        }

        allocator_type get_allocator() const {
            return allocator_type();
        }

        reference at(size_type pos) {
            if (pos >= size()) {
                throw std::out_of_range("Vector:pos >= size() in at");
            } else {
                return operator[](pos);
            }
        }

        const_reference at(size_type pos) const {
            if (pos >= size()) {
                throw std::out_of_range("Vector:pos >= size() in at");
            } else {
                return operator[](pos);
            }
        }

        reference operator[](size_type pos) {
            return start[pos];
        }

        const_reference operator[](size_type pos) const {
            return start[pos];
        }

        reference front() {
            return *begin();
        }

        const_reference front() const {
            return *begin();
        }

        reference back() {
            return *(end() - 1);
        }

        const_reference back() const {
            return *(end() - 1);
        }

        T *data() noexcept {
            return start;
        }

        const T *data() const noexcept {
            return start;
        }

        iterator begin() noexcept {
            return start;
        }

        const_iterator begin() const {
            return cbegin();
        }

        const_iterator cbegin() const noexcept {
            return start;
        }

        iterator end() noexcept {
            return finish;
        }

        const_iterator end() const noexcept {
            return cend();
        }

        const_iterator cend() const noexcept {
            return finish;
        }

        reverse_iterator rbegin() noexcept {
            return reverse_iterator(end());
        }

        const_reverse_iterator rbegin() const noexcept {
            return crbegin();
        }

        const_reverse_iterator crbegin() const noexcept {
            return reverse_iterator(end());
        }

        bool empty() const noexcept {
            return begin() == end();
        }

        size_type size() const noexcept {
            return finish - start;
        }

        size_type max_size() const noexcept {
            return SIZE_MAX;
        }

        void reserve(size_type need) {
            auto now_capacity = capacity();
            if (now_capacity == 0) {
                expand_space_to(1);
            } else if (now_capacity < need) {
                while (now_capacity < need) {
                    now_capacity *= 2;
                }
                expand_space_to(now_capacity);
            }
        }

        size_type capacity() const noexcept {
            return end_of_storage - start;
        }

        void shrink_to_fit() {
            auto need = size();
            auto new_start = allocator_type::allocate(need);
            finish = uninitialized_move(start, finish, new_start);
            allocator_type::deallocate(start);
            start = new_start;
            end_of_storage = finish;
        }

        void clear() noexcept {
            Readable::destroy(begin(), end());
            finish = start;
        }

    private:
        void move_elements_back(const_iterator pos, size_type move_distance) {
            Readable::move_backward(pos, const_iterator(finish - move_distance), finish);
        }

        void move_elements_forward(const_iterator pos, size_type move_distance) {
            Readable::move(iterator(pos), finish, iterator(pos - move_distance));
        }

    public:
        iterator insert(const_iterator pos, const T &value) {
            auto distance_from_begin = pos - start;
            reserve(size() + 1);
            ++finish;
            auto it = start + distance_from_begin;
            move_elements_back(it, 1);
            *it = value;
            return it;
        }

        iterator insert(const_iterator pos, T &&value) {
            auto distance_from_begin = pos - start;
            reserve(size() + 1);
            ++finish;
            auto it = start + distance_from_begin;
            move_elements_back(it, 1);
            *it = std::move(value);
            return it;
        }

    private:
        iterator insert(const_iterator pos, size_type count, const T &value, Readable::true_type) {
            auto distance_from_begin = pos - start;
            reserve(size() + count);
            finish += count;
            auto it = start + distance_from_begin;
            move_elements_back(it, count);
            Readable::uninitialized_fill(it, it + count, value);
            return it;
        }

        template<typename InputIt>
        iterator insert(const_iterator pos, InputIt first, InputIt last, Readable::false_type) {
            auto distance_from_begin = pos - start;
            auto count = Readable::distance(first, last);
            reserve(size() + count);
            finish += count;
            auto it = start + distance_from_begin;
            move_elements_back(it, static_cast<size_type>(count));
            Readable::uninitialized_copy(first, last, it);
            return it;
        }

    public:
        iterator insert(const_iterator pos, size_type count, const T &value) {
            return insert(pos, count, value, Readable::true_type());
        }

        template<typename InputItOrInteger>
        iterator insert(const_iterator pos, InputItOrInteger first, InputItOrInteger last) {
            return insert(pos, first, last, Readable::is_integral<InputItOrInteger>());
        }

        iterator insert(const_iterator pos, const std::initializer_list<T> &ilist) {
            return insert(pos, ilist.begin(), ilist.end());
        }

        template<typename... Args>
        iterator emplace(const_iterator pos, Args &&... args) {
            auto distance_from_begin = pos - start;
            reserve(size() + 1);
            ++finish;
            auto it = start + distance_from_begin;
            move_elements_back(it, 1);
            *it = allocator_type::construct(std::forward<Args>(args)...);
            return it;
        }

        iterator erase(const_iterator pos) {
            Readable::destroy(pos);
            move_elements_forward(pos, 1);
            --finish;
            return iterator(pos);
        }

        iterator erase(const_iterator first, const_iterator last) {
            Readable::destroy(first, last);
            auto n = static_cast<size_type>(distance(first, last));
            move_elements_forward(first, n);
            finish -= n;
            return iterator(first);
        }

        void push_back(const T &value) {
            reserve(size() + 1);
            *finish = value;
            ++finish;
        }

        void push_back(T &&value) {
            reserve(size() + 1);
            *finish = std::move(value);
            ++finish;
        }

        template<class... Args>
        reference emplace_back(Args &&... args) {
            reserve(size() + 1);
            allocator_type::construct(finish, std::forward<Args>(args)...);
            ++finish;
            return *finish;
        }

        void pop_back() {
            --finish;
            allocator_type::destroy(finish);
        }

        void resize(size_type count) {
            if (count < size()) {
                insert(end(), size() - count, value_type());
            } else if (count > size()) {
                erase(begin() + count, end());
            }
        }

        void resize(size_type count, const value_type &value) {
            if (count < size()) {
                insert(end(), size() - count, value);
            } else if (count > size()) {
                erase(begin() + count, end());
            }
        }

        void swap(vector &other) {
            std::swap(start, other.start);
            std::swap(finish, other.finish);
            std::swap(end_of_storage, other.end_of_storage);
        }
    };

    template<typename T, typename Alloc1, typename Alloc2>
    int compare(const vector<T, Alloc1> &lhs,
                const vector<T, Alloc2> &rhs) {
        auto it_lhs = lhs.begin();
        auto it_rhs = rhs.begin();
        for (; it_lhs != lhs.end() && it_rhs != rhs.end(); ++it_lhs, ++it_rhs) {
            if (*it_lhs < *it_rhs) {
                return -1;
            } else if (*it_lhs > *it_rhs) {
                return 1;
            }
        }
        if (it_lhs != lhs.end() && it_rhs == rhs.end()) {
            return 1;
        } else if (it_lhs == lhs.end() && it_rhs != rhs.end()) {
            return -1;
        }
        return 0;
    }

    template<typename T, typename Alloc1, typename Alloc2>
    bool operator==(const vector<T, Alloc1> &lhs,
                    const vector<T, Alloc2> &rhs) {
        return compare(lhs, rhs) == 0;
    }


    template<typename T, typename Alloc1, typename Alloc2>
    bool operator!=(const vector<T, Alloc1> &lhs,
                    const vector<T, Alloc2> &rhs) {
        return compare(lhs, rhs) != 0;
    }


    template<typename T, typename Alloc1, typename Alloc2>
    bool operator<(const vector<T, Alloc1> &lhs,
                   const vector<T, Alloc2> &rhs) {
        return compare(lhs, rhs) < 0;
    }


    template<typename T, typename Alloc1, typename Alloc2>
    bool operator<=(const vector<T, Alloc1> &lhs,
                    const vector<T, Alloc2> &rhs) {
        return compare(lhs, rhs) <= 0;
    }


    template<typename T, typename Alloc1, typename Alloc2>
    bool operator>(const vector<T, Alloc1> &lhs,
                   const vector<T, Alloc2> &rhs) {
        return compare(lhs, rhs) > 0;
    }


    template<typename T, typename Alloc1, typename Alloc2>
    bool operator>=(const vector<T, Alloc1> &lhs,
                    const vector<T, Alloc2> &rhs) {
        return compare(lhs, rhs) >= 0;
    }

    template<typename T, typename Alloc1, typename Alloc2>
    void swap(vector<T, Alloc1> &lhs,
              vector<T, Alloc2> &rhs) {
        lhs.swap(rhs);
    };
}

#endif //STL_FROM_SCRATCH_VECTOR_H
