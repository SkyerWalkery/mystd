#pragma once
#include <functional>
#include "Iterator.h"

namespace mystd {


template<typename InputIterator, typename Cmp>
void quickSort(InputIterator begin, InputIterator end, Cmp cmp) {
    if (begin >= end)
        return;
    using value_type = typename iterator_traits<InputIterator>::value_type;
    using std::swap;

    InputIterator i = begin, j = --end;
    value_type pivot = *begin;
    while (i < j) {
        while (i < j && cmp(pivot, *j)) { --j; }
        while (i < j && !cmp(pivot, *i)) { ++i; }
        if (i < j)
            swap(*i, *j);
    }
    swap(*begin, *i);

    quickSort(begin, i, cmp);
    quickSort(++i, ++end, cmp);
}

template <typename InputIterator>
void quickSort(InputIterator begin, InputIterator end) {
    using value_type = typename iterator_traits<InputIterator>::value_type;
    quickSort(begin, end, std::less<value_type>());
}


template <typename T> 
constexpr const T& max(const T& a, const T& b) {
    return a < b ? b : a;
}




template <typename InputIterator, typename T>
InputIterator find(InputIterator begin, InputIterator end, const T& value) {
    while (begin != end && *begin != value)
        ++begin;
    return begin;
}



//heap
/*
Given a heap in the range[first, last - 1),
this function extends the range considered a heap to [first,last) 
by placing the value in (last-1) into its corresponding location within it.
*/
template <typename RandomAccessIterator, typename Compare>
void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    using value_type = typename iterator_traits<RandomAccessIterator>::value_type;

    if (last - first < 2) return;

    value_type elem = *(last - 1);
    auto elem_idx = last - first - 1;
    using idx_type = decltype(elem_idx);
    idx_type root_idx = 0;
    idx_type parent_idx = (elem_idx - 1) / 2;

    while (elem_idx > root_idx && comp(*(first + parent_idx), elem)){
        *(first + elem_idx) = *(first + parent_idx);
        elem_idx = parent_idx;
        parent_idx = (elem_idx - 1) / 2;
    }
    *(first + elem_idx) = elem;
}


template <typename RandomAccessIterator>
void push_heap(RandomAccessIterator first, RandomAccessIterator last){
    using value_type = typename iterator_traits<RandomAccessIterator>::value_type;
    mystd::push_heap(first, last, std::less<value_type>());
}


template <typename RandomAccessIterator, typename Distance, typename Compare>
void fix_down(RandomAccessIterator beg, Distance size, Distance start_idx, Compare comp){
    using value_type = typename iterator_traits<RandomAccessIterator>::value_type;

    value_type elem = *(beg + start_idx);
    Distance node_idx = start_idx;
    Distance right_child = start_idx;

    while (right_child * 2 + 2 < size){
        right_child = 2 * right_child + 2;

        if (comp(*(beg + right_child), *(beg + right_child - 1))){
            --right_child;
        }
        if (!comp(elem, *(beg + right_child))){
            break;
        }

        *(beg + node_idx) = *(beg + right_child);
        node_idx = right_child;
    }

    if (right_child * 2 + 2 == size){
        right_child = 2 * right_child + 1;
        if (comp(elem, *(beg + right_child))){
            *(beg + node_idx) = *(beg + right_child);
            node_idx = right_child;
        }
    }
    *(beg + node_idx) = elem;
}

template <typename RandomAccessIterator, typename Distance>
void fix_down(RandomAccessIterator beg, Distance size, Distance start_idx){
    using value_type = typename iterator_traits<RandomAccessIterator>::value_type;
    mystd::fix_down(beg, size, start_idx, std::less<value_type>());
}


template <typename RandomAccessIterator, typename Compare>
void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    if (last - first < 2){
        return;
    }
    std::swap(*first, *(--last));
    auto size = last - first;
    decltype(size) index = 0;
    mystd::fix_down(first, size, index, comp);
}


template <typename RandomAccessIterator>
void pop_heap(RandomAccessIterator first, RandomAccessIterator last){
    using value_type = typename iterator_traits<RandomAccessIterator>::value_type;
    mystd::pop_heap(first, last, std::less<value_type>());
}


template <typename RandomAccessIterator, typename Compare>
void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp){
    if (last - first < 2) {
        return;
    }
    auto size = last - first;
    auto last_idx = size - 1;
    for (auto index = (last_idx - 1) / 2; index >= 0; --index)
    {
        mystd::fix_down(first, size, index, comp);
    }
}


template <typename RandomAccessIterator>
void make_heap(RandomAccessIterator first, RandomAccessIterator last){
    using value_type = typename iterator_traits<RandomAccessIterator>::value_type;
    mystd::make_heap(first, last, std::less<value_type>());
}


template<typename RandomAccessIterator, typename Compare>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp){
    while (last - first > 1){
        mystd::pop_heap(first, last--, comp);
    }
}

template<typename RandomAccessIterator>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last){
    using value_type = typename iterator_traits<RandomAccessIterator>::value_type;
    mystd::sort_heap(first, last, std::less<value_type>());
}







}