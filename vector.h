#pragma once

#include <memory>

namespace mystd {
using std::allocator;

template<typename T>
class vector {
public:
    using value_type =          T;
    using pointer =             T*;
    using const_pointer =       const T*;
    using reference =           T&;
    using const_reference =     const T&;
    using iterator =            T*;
    using const_iterator =      const T*;
    using size_type =           std::size_t;
    using difference_type =     std::ptrdiff_t;

private:
    const size_type INIT_CAPACITY = 1;
    const size_type EXPAND_RATE = 2;

private:
    allocator<T> alloc_;
    pointer elem_ = nullptr; //elements of vector
    pointer end_ = nullptr; //point to position after last elem
    pointer free_ = nullptr; //the first space after capacity pf vector

private:
    void destroyElem(iterator begin, iterator end) {
        for (iterator it = begin; it < end; ++it)
            alloc_.destroy(it);
    }

    //destroy elements and free the space
    void clearMem() {
        if (elem_) {
            destroyElem(elem_, end_);
            alloc_.deallocate(elem_, capacity());
            elem_ = free_ = end_ = nullptr;
        }
    }

    void expandCapacity(size_type new_capacity) {
        if (new_capacity <= capacity())
            return;

        pointer new_elem_ = nullptr, new_end_, new_free_;
        try {
            new_elem_ = alloc_.allocate(new_capacity);
        }
        catch (...) {
            alloc_.deallocate(new_elem_, new_capacity);
            throw;
        }
        new_end_ = new_elem_ + size();
        new_free_ = new_elem_ + new_capacity;
        std::uninitialized_copy(elem_, end_, new_elem_);
        clearMem();
        elem_ = new_elem_;
        end_ = new_end_;
        free_ = new_free_;
    }

    void checkCapacity() {
        if (end_ == free_)
            expandCapacity(empty() ? INIT_CAPACITY : capacity() * EXPAND_RATE);
    }

public:
    /******constructor******/
    //default
    explicit vector() = default;

    //fill
    explicit vector(size_type n, const value_type& val = value_type()) {
        try {
            elem_ = alloc_.allocate(n);
        }
        catch (...) {
            alloc_.deallocate(elem_, n);
            throw;
        }
        std::uninitialized_fill_n(elem_, n, val);
        end_ = free_ = elem_ + n;
    }

    //to simplify my vector, it only requires vector::iterator
    vector(const_iterator first, const_iterator last) {
        if (first == nullptr && last == nullptr)
            return;
        if (first > last)
            throw std::out_of_range("at vector()");

        difference_type n = std::distance(first, last);
        pointer new_elem_;
        new_elem_ = alloc_.allocate(n);
        try {
            std::uninitialized_copy(first, last, new_elem_);
        }
        catch (...) {
            alloc_.deallocate(new_elem_, n);
            throw;
        }

        clearMem();
        elem_ = new_elem_;
        end_ = free_ = elem_ + n;
    }

    vector(const vector& other) : vector(other.cbegin(), other.cend()) {}

    vector(vector&& other) noexcept {
        swap(other);
    }

    vector& operator=(const vector& other) {
        if (&other == this)
            return *this;
        vector copy(other);
        swap(copy);
        return *this;
    }

    vector& operator=(vector&& other) {
        if (&other == this)
            return *this;
        swap(other);
        return *this;
    }

    //TO DO: construct with an initializer_list

    /******Destructor******/
    ~vector() {
        clearMem();
    }


    /******Capacity******/
    size_type size() const noexcept {
        return end_ - elem_;
    }

    size_type capacity() const noexcept {
        return free_ - elem_;
    }

    bool empty() const noexcept {
        return end_ == elem_;
    }

    void reserve(size_type n) {
        expandCapacity(n);
    }

    void resize(size_type n) {
        resize(n, value_type());
    }

    void resize(size_type n, const value_type& val) {
        if (n < size()) {
            erase(begin() + n, end());
        }
        else if (n > size()) {
            insert(end(), size() - n, val);
        }
    }
    //TO DO:

    /******Element access******/
    reference operator[](size_type index) {
        return elem_[index];
    }

    const_reference operator[](size_type index) const {
        return const_cast<vector*>(this)->operator[](index);
    }

    reference at(size_type index) {
        if (index >= size())
            throw std::out_of_range("at operator[]");
        return this->operator[](index);
    }

    const_reference at(size_type index) const {
        return const_cast<vector*>(this)->at(index);
    }

    reference front() {
        return *begin();
    }

    const_reference front() const{
        return const_cast<vector*>(this)->front();
    }

    reference back() {
        if (!elem_)
            throw std::out_of_range("at back()");
        return *(end_ - 1);
    }

    const_reference back() const{
        if (!elem_)
            throw std::out_of_range("at back()");
        return const_cast<vector*>(this)->back();
    }

    //TO DO: const_reference versions

    /******iterator******/
    iterator begin() noexcept {
        return elem_;
    }

    const_iterator begin() const noexcept {
        return elem_;
    }

    const_iterator cbegin() const noexcept {
        return elem_;
    }

    iterator end() noexcept {
        return end_;
    }

    const_iterator end() const noexcept {
        return end_;
    }

    const_iterator cend() const noexcept {
        return end_;
    }
    //TO DO: rbegin, rend

    /******Modifiers******/
    void push_back(const value_type& val) {
        checkCapacity();
        alloc_.construct(end_, val);
        ++end_;
    }

    void push_back(value_type&& val) {
        checkCapacity();
        new(end_) value_type(std::move(val));
        ++end_;
    }

    void pop_back() {
        if (empty())
            throw std::out_of_range("at pop_back()");
        alloc_.destroy(end_);
        --end_;
    }

    iterator insert(const_iterator position, const value_type& val) {
        value_type val_ = val;
        return insert(position, std::move(val_));
    }

    iterator insert(const_iterator position, size_type n, const value_type& val) {
        iterator non_const_pos = const_cast<iterator>(position);
        if (n == 0)
            return non_const_pos;
        for (size_type i = 0; i < n; ++i) {
            non_const_pos = insert(non_const_pos, val);
        }
        return non_const_pos;
    }

    iterator insert(const_iterator position, value_type&& val) {
        if (position < cbegin() || position > cend()) {
            throw std::out_of_range("at insert()");
        }
        //if vector is expanded, position will be valid
        difference_type idx = position - cbegin();
        checkCapacity();
        iterator non_const_pos = begin() + idx;

        if (non_const_pos == end()) {
            alloc_.construct(end_, val);
            iterator ret = end();
            ++end_;
            return ret;
        }

        alloc_.construct(end_, *(end_ - 1));
        ++end_;
        for (iterator it = end() - 2; it > non_const_pos; --it) {
            *it = std::move(*(it - 1));
        }
        *non_const_pos = val;
        return non_const_pos;
    }

    //[first, last)
    iterator erase(const_iterator first, const_iterator last) {
        if (empty() || first < cbegin() || last > cend())
            throw std::out_of_range("at erase()");

        if (last == cend()) {
            destroyElem(const_cast<iterator>(first), end_);
            end_ = const_cast<iterator>(first);
            return end();
        }

        iterator iter = std::move(const_cast<iterator>(last), end_, const_cast<iterator>(first));
        destroyElem(iter, end_);
        end_ = iter;
        return const_cast<iterator>(first);
    }

    iterator erase(const_iterator position) {
        if (position == cend()) {
            alloc_.destroy(--end_);
            return end();
        }
        return erase(position, position + 1);
    }

    void clear() noexcept {
        if (empty())
            throw std::out_of_range("at clear()");
        destroyElem(elem_, end_);
        end_ = elem_;
    }

    void swap(vector& other) {
        std::swap(elem_, other.elem_);
        std::swap(end_, other.end_);
        std::swap(free_, other.free_);
    }

    //TO DO: insert(range), assign
};

}
