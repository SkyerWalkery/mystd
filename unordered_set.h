#pragma once
#include <forward_list>
#include <vector>
#include <utility>
#include <algorithm>
#include "vector.h"
#include "iterator.h"
#include "algorithm.h"

//#define USING_STD_VECTOR
#define USING_STD_LIST


namespace mystd {

template<typename T, typename Hash = std::hash <T>, typename Equal = std::equal_to<T>>
class unordered_set {
private:
#ifdef USING_STD_LIST
    using bucket_type = std::forward_list<T>;
#else
    using bucket_type = mystd::vector<T>;
#endif

#ifdef USING_STD_VECTOR
    using vector_type = std::vector<bucket_type>;
#else
    using vector_type = mystd::vector<bucket_type>;
#endif

public:
    using key_type = T;
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using hasher = Hash;
    using key_equal = Equal;
    using local_iterator = typename bucket_type::const_iterator;
    using const_local_iterator = typename bucket_type::const_iterator;

public:

    class const_iterator {
        friend class unordered_set;
    public:
        using value_type = T;
        using pointer = const T*;
        using reference = const T&;
        using difference_type = std::ptrdiff_t;
        using iterator_category = mystd::forward_iterator_tag;

    public:
        const_iterator() :ptr_(nullptr), bucket_idx_(0) {}

        reference operator*() const {
            return *iter_;
        }

        pointer operator->() const {
            return &(operator*());
        }

        const_iterator& operator++() noexcept {
            ++iter_;
            while (bucket_idx_ < ptr_->size() && iter_ == (*ptr_)[bucket_idx_].cend()) {
                if(++bucket_idx_ < ptr_->size())
                    iter_ = (*ptr_)[bucket_idx_].cbegin();
            }
            return *this;
        }

        const_iterator operator++(int) noexcept {
            const_iterator ret = *this;
            ++(*this);
            return ret;
        }

        bool operator==(const const_iterator& other) const noexcept {
            return ptr_ == other.ptr_ && bucket_idx_ == other.bucket_idx_ && iter_ == other.iter_;
        }

        bool operator!=(const const_iterator& other) const noexcept {
            return !(*this == other);
        }

    protected:
        const_iterator(vector_type* ptr, bool is_end):ptr_(ptr) {
            if (is_end) {
                bucket_idx_ = ptr_->size();
                if (!ptr_->empty())
                    iter_ = (*ptr_)[bucket_idx_ - 1].cend();
            }
            
            else {
                bucket_idx_ = 0;
                iter_ = (*ptr)[bucket_idx_].cbegin();
                while (bucket_idx_ < ptr_->size() && iter_ == (*ptr_)[bucket_idx_].cend()) {
                    if (++bucket_idx_ < ptr_->size())
                        iter_ = (*ptr_)[bucket_idx_].cbegin();
                }
            }
        }

        const_iterator(vector_type* ptr, size_type bucket_idx, local_iterator iter) :
            ptr_(ptr), bucket_idx_(bucket_idx), iter_(iter) {}

    private:
        vector_type* ptr_;
        size_type bucket_idx_;
        local_iterator iter_;
    };

    //key in unordered_set cannot be changed
    using iterator = const_iterator;

private:
    hasher hash_;
    key_equal equal_;
    vector_type buckets_;
    size_type size_ = 0;
    float max_load_factor_ = 1.0; //Max average no. of elements per bucket
    float growth_factor_ = 2.0; //expand factor

    static const size_type PRIME_ARR_SIZE = 28;
    static const size_type prime_[PRIME_ARR_SIZE];

    size_type next_prime(size_type n)const {
        const size_type* prime_arr_end = std::end(prime_);
        const size_type* prime_ptr = std::lower_bound(prime_, prime_arr_end, n);
        if (prime_ptr != prime_arr_end)
            return *prime_ptr;
        else
            return prime_[PRIME_ARR_SIZE - 1];
    }

public:
    /******constructor, destructor and copy******/
    //default and empty
    unordered_set() :unordered_set(static_cast<size_type>(0)) {}

    explicit unordered_set(size_type n, const hasher& hf = hasher(), const key_equal& eql = key_equal()) :
        buckets_(next_prime(n), bucket_type()), hash_(hf), equal_(eql) {}

    //copy
    unordered_set(const unordered_set& other) :
        buckets_(other.buckets_), size_(other.size_), max_load_factor_(other.max_load_factor_) {}

    //move
    unordered_set(unordered_set&& other) noexcept {
        swap(other);
    }
//TO DO: range, initializer list

    unordered_set& operator=(const unordered_set& other) {
        if (this == &other)
            return *this;
        unordered_set copy(other);
        swap(copy);
        return *this;
    }

    unordered_set& operator=(unordered_set&& other) noexcept {
        if (this == &other)
            return *this;
        swap(other);
        return *this;
    }
//TO DO: initializer list

    ~unordered_set() = default;

    /******Capacity******/
    size_type size() const noexcept {
        return size_;
    }

    size_type max_size()const noexcept {
        return prime_[PRIME_ARR_SIZE - 1];
    }

    bool empty() const noexcept {
        return size_ == 0;
    }

    /******Iterators******/
    //container iterator
    iterator begin() noexcept {
        return const_iterator(&buckets_, false);
    }

    const_iterator begin()const noexcept {
        return const_cast<unordered_set*>(this)->begin();
    }

    iterator end() noexcept {
        return const_iterator(&buckets_, true);
    }

    const_iterator end()const noexcept {
        return const_cast<unordered_set*>(this)->end();
    }

    const_iterator cbegin()const noexcept {
        return begin();
    }

    const_iterator cend()const noexcept {
        return end();
    }

    //bucket iterator
    //Returns an iterator pointing to the first element in one of its buckets
    local_iterator begin(size_type n) {
        return buckets_[n].begin();
    }

    const_local_iterator begin(size_type n)const {
        return buckets_[n].begin();
    }

    local_iterator end(size_type n) {
        return buckets_[n].end();
    }

    const_local_iterator end(size_type n)const {
        return buckets_[n].end();
    }

    const_local_iterator cbegin(size_type n)const {
        return buckets_[n].cbegin();
    }

    const_local_iterator cend(size_type n)const {
        return buckets_[n].cend();
    }

    /******Element lookup******/
    iterator find(const key_type& k) {
        const size_type pos = bucket(k);
        local_iterator iter = mystd::find(buckets_[pos].cbegin(), buckets_[pos].cend(), k);
        if (iter != cend(pos))
            return const_iterator(&buckets_, pos, iter);
        else
            return end();
    }

    const_iterator find(const key_type& k) const {
        return const_cast<unordered_set*>(this)->find(k);
    }

    size_type count(const key_type& k) const {
        const size_type pos = bucket(k);
        local_iterator iter = mystd::find(buckets_[pos].cbegin(), buckets_[pos].cend(), k);
        if (iter != cend(pos)) return 1;
        else return 0;
    }

    std::pair<iterator, iterator> equal_range(const key_type& k) {
        iterator ret = this->find(k);
        return std::make_pair(ret, ret);
    }

    /******Modifiers******/
//TO DO: insert(3)(4)(5)(6) in cpp reference, emplace

    std::pair<iterator, bool> insert(const value_type& val) {
        //rehash when necessary
        
        std::pair<bool, size_type> need_rh = need_rehash(size() + 1);
        if (need_rh.first) {
            rehash(need_rh.second);
        }
        
        const size_type pos = bucket(val);
        local_iterator iter = mystd::find(buckets_[pos].cbegin(), buckets_[pos].cend(), val);
        //if the element exits
        if (iter != cend(pos)) {
            return std::make_pair(iterator(&buckets_, pos, iter), false);
        }
        //not found, then insert
        else {
            buckets_[pos].push_front(val);
            ++size_;
            return std::make_pair(iterator(&buckets_, pos, buckets_[pos].cbegin()), true);
        }
    }

    std::pair<iterator, bool> insert(value_type&& val) {
        const value_type copy = val;
        return insert(copy);
    }

    iterator erase(const_iterator position) {
        if (position == cend())
            throw std::out_of_range("at mystd::unordered_set::erase()");

        const value_type rm_val = *position;
        const size_type idx = position.bucket_idx_;
        ++position;
        --size_;
        buckets_[idx].remove(rm_val);
        return position;
    }

    size_type erase(const key_type& k) {
        const size_type pos = bucket(k);
        local_iterator iter = mystd::find(buckets_[pos].cbegin(), buckets_[pos].cend(), k);
        //if the element exits
        if (iter == cend(pos)) {
            return 0;
        }
        //not found, then insert
        else {
            buckets_[pos].remove(k);
            --size_;
            return 1;
        }
    }

    iterator erase(const_iterator first, const_iterator last) {
        const_iterator it = first;
        while (it != last)
            it = erase(it);
        return last;
    }

    void swap(unordered_set& other) noexcept {
        using std::swap;
        buckets_.swap(other.buckets_);
        swap(size_, other.size_);
        swap(max_load_factor_, other.max_load_factor_);
    }

    void clear()noexcept {
        buckets_.clear();
        size_ = 0;
    }

    /******Buckets******/
    size_type bucket_count()const noexcept {
        return buckets_.size();
    }

    size_type max_bucket_count()const noexcept {
        return prime_[PRIME_ARR_SIZE - 1];
    }

    //Returns the number of elements in bucket n
    size_type bucket_size(size_type n) const {
        return buckets_[n].size();
    }

    //Returns the bucket number where the element with value k is located
    size_type bucket(const key_type& k) const {
        return hash_(k) % bucket_count();
    }

    /******Hash policy******/
    float load_factor()const noexcept {
        return static_cast<float>(size()) / static_cast<float>(bucket_count());
    }

    float max_load_factor() const noexcept {
        return max_load_factor_;
    }

    void max_load_factor(float z) {
        max_load_factor_ = z;
    }

    void rehash(size_type n) {
        if (n < bucket_count())
            return;
        const size_type new_bucket_cnt = next_prime(n);
        if (new_bucket_cnt > bucket_count()) {
            unordered_set expanded_copy(new_bucket_cnt);
            for (const value_type& val : *this)
                expanded_copy.insert(std::move(val));
            buckets_.swap(expanded_copy.buckets_);
        }
    }

    void reserve(size_type n) {
        std::pair<bool, size_type> rh = need_rehash(n);
        if (rh.first)
            rehash(rh.second);
    }

private:
    std::pair<bool, size_type> need_rehash(const size_type& next_size)const {
        //refer to http://hustsxh.is-programmer.com/posts/82605.html
        //need at least min buckets to contain size() + 1 elements
        float min_bkts = static_cast<float>(next_size + 1) / max_load_factor_;
        if (min_bkts > bucket_count()) {
            min_bkts = mystd::max(min_bkts, growth_factor_ * size());
            size_type next_resize = next_prime(min_bkts);
            return std::make_pair(true, next_resize);
        }
        return std::make_pair(false, 0);
    }

};

template <typename T, typename Hash, typename Equal>
const typename unordered_set<T, Hash, Equal>::size_type
unordered_set<T, Hash, Equal>::prime_[PRIME_ARR_SIZE] = {
        53u, 97u, 193u, 389u, 769u, 1543u, 3079u, 6151u, 12289u, 24593u, 49157u,
        98317u, 196613u, 393241u, 786433u, 1572869u, 3145739u, 6291469u, 12582917u,
        25165843u, 50331653u, 100663319u, 201326611u, 402653189u, 805306457u,
        1610612741u, 3221225473u, 4294967291u
};

}




