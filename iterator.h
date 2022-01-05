#pragma once

namespace mystd {
struct output_iterator_tag
{
};

struct input_iterator_tag
{
};

struct forward_iterator_tag : public input_iterator_tag
{
};

struct bidirectional_iterator_tag : public forward_iterator_tag
{
};

struct random_access_iterator_tag : public bidirectional_iterator_tag
{
};


//萃取
template <typename T>
struct iterator_traits
{
    using iterator_category = typename T::iterator_category;
    using value_type = typename T::value_type;
    using difference_type = typename T::difference_type;
    using pointer = typename T::pointer;
    using reference = typename T::reference;
};

//for common pointer, vector::Iterator, for example
template <typename T>
struct iterator_traits<T*>
{
    using iterator_category = random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;
};

template <typename T>
struct iterator_traits<const T*>
{
    using iterator_category = random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;
};
}