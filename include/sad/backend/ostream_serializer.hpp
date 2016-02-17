// The MIT License (MIT)
//
// Copyright (c) 2015 Jeremy Letang
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef __SAD__BACKEND_OSTREAM_SERIALIZER__20160213__
#define __SAD__BACKEND_OSTREAM_SERIALIZER__20160213__

#include <iostream>
#include <algorithm>
#include <type_traits>
#include <stack>
#include <queue>

#include "../schema.hpp"
#include "../utility.hpp"
#include "../tuple_utils.hpp"
#include "../type_traits.hpp"
#include "../maybe_null.hpp"
#include "../container_adapter_helper.hpp"

namespace sad {
namespace backend {
namespace detail {
namespace basic_ostream {

// declarations

// std::basic_string
template <typename CharT, typename Traits, typename Allocator>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os,
                              const std::basic_string<CharT, Traits, Allocator>& s);
// char
template <typename CharT, typename Traits>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os, const char& c);
// numbers
template <typename CharT,
          typename Traits,
          typename T,
          typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os, const T& t);
// any object
template <typename CharT,
          typename Traits,
          typename T,
          typename std::enable_if<!std::is_arithmetic<T>::value &&
                                  !sad::traits::is_maybe_null_v<T> &&
                                  !sad::traits::has_iterator_v<T>>::type* = nullptr>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os, const T& t);
// containers (std::list, std::vector, std::deque, std::forward_list ...)
template <typename CharT,
          typename Traits,
          template<typename, typename> class C,
          typename T,
          typename Allocator = std::allocator<T>>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os, const C<T, Allocator>& t);
// std::pair
template <typename CharT, typename Traits, typename T1, typename T2>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os, const std::pair<T1, T2>& p);
// std::tuple
template <typename CharT, typename Traits, typename... Tys>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os, const std::tuple<Tys...>& t);
// sad::maybe_null
template <typename CharT, typename Traits, typename T>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os, const sad::maybe_null<T>& mt);
// std::array<T, std::size_t N> support
template <typename CharT,
          typename Traits,
          template <typename, std::size_t> class Array,
          typename T,
          std::size_t N>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os, const Array<T, N>& a);
// std::stack
template <typename CharT, typename Traits, typename T, typename Container>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os,
                              const std::stack<T, Container>& s);
// std::queue
template <typename CharT, typename Traits, typename T, typename Container>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os,
                              const std::queue<T, Container>& q);
// std::priority_queue
template <typename CharT, typename Traits, typename T, typename Container>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os,
                              const std::priority_queue<T, Container>& pq);
// std::unordered_map / std::unorederd_multimap
template <typename CharT,
          typename Traits,
          template <typename, typename, typename, typename, typename> class UMap,
          typename Key,
          typename T,
          typename Hash = std::hash<Key>,
          typename KeyEqual = std::equal_to<Key>,
          typename Allocator = std::allocator<std::pair<const Key, T>>>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os,
                              const UMap<Key, T, Hash, KeyEqual, Allocator>& m);
// std::map / std::multimap
template <typename CharT,
          typename Traits,
          template <typename, typename, typename, typename> class Map,
          typename Key,
          typename T,
          typename Compare = std::less<Key>,
          typename Allocator = std::allocator<std::pair<const Key, T>>>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os,
                              const Map<Key, T, Compare, Allocator>& m);
// std::set / std::multset
template <typename CharT,
          typename Traits,
          template <typename, typename, typename> class Set,
          typename Key,
          typename Compare = std::less<Key>,
          typename Allocator = std::allocator<Key>>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os,
                              const Set<Key, Compare, Allocator>& s);
// schema
template<typename CharT, typename Traits, typename... Types>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os,
                              const sad::schema_mapper<Types...>& schema);

// definitions

// std::basic_string
template <typename CharT, typename Traits, typename Allocator>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os,
                              const std::basic_string<CharT, Traits, Allocator>& s) {
    os << "\"" << s << "\"";
}

// char
template <typename CharT, typename Traits>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os, const char& c) {
  os << "'" << c << "'";
}

// numbers
template <typename CharT,
          typename Traits,
          typename T,
          typename std::enable_if<std::is_arithmetic<T>::value>::type*>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os, const T& t) {
    if (sad::traits::is_bool_v<T>) { os << std::boolalpha << t; }
    else { os << t; };
}

// any objects
template <typename CharT,
          typename Traits,
          typename T,
          typename std::enable_if<!std::is_arithmetic<T>::value &&
                                  !sad::traits::is_maybe_null_v<T> &&
                                  !sad::traits::has_iterator_v<T>>::type*>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os, const T& t) {
    print_field_value(os, sad::schema<T>()(t));
}

// containers
template <typename CharT,
          typename Traits,
          template<typename, typename> class C,
          typename T,
          typename Allocator>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os, const C<T, Allocator>& t) {
    auto first = true;
    os << "[";
    std::for_each(t.cbegin(), t.cend(), [&first, &os](const auto& e) {
        if (not first) { os << ", "; }
        else { first = false; }
        print_field_value(os, e);
    });
    os << "]";
}

// pairs
template <typename CharT, typename Traits, typename T1, typename T2>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os,
                              const std::pair<T1, T2>& p) {
    os << "{'first': ";
    print_field_value(os, p.first);
    os << ", 'second': ";
    print_field_value(os, p.second);
    os << "}";
}

// tuples
template <typename CharT, typename Traits, typename... Tys>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os,
                              const std::tuple<Tys...>& t) {
    auto first = true;
    os << "(";
    auto f = [&first, &os](const auto& e) {
        if (not first) { os << ", "; }
        else { first = false; }
        print_field_value(os, e);
    };
    sad::tuple_utils::for_each(t, f);
    os << ")";
}

// sad::maybe_null
template <typename CharT, typename Traits, typename T>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os,
                              const sad::maybe_null<T>& mt) {
    if (mt.is_null()) { os << "null"; } 
    else { print_field_value(os, *mt); }
}

// std::array<T, std::size_t N> support
template <typename CharT,
          typename Traits,
          template <typename, std::size_t> class Array,
          typename T,
          std::size_t N>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os, const Array<T, N>& a) {
  auto first = true;
    os << "[";
    std::for_each(a.cbegin(), a.cend(), [&first, &os](const auto& e) {
        if (not first) { os << ", "; }
        else { first = false; }
        print_field_value(os, e);
    });
    os << "]";
}

// stack / queue / priority_queue

// std::stack
template <typename CharT, typename Traits, typename T, typename Container>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os,
                              const std::stack<T, Container>& s) {
    auto& c = sad::helper::get_internal_container(s);
    print_field_value(os, c);
}

// std::queue
template <typename CharT, typename Traits, typename T, typename Container>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os,
                              const std::queue<T, Container>& q) {
    auto& c = sad::helper::get_internal_container(q);
    print_field_value(os, c);
}

// std::priority_queue
template <typename CharT, typename Traits, typename T, typename Container>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os,
                              const std::priority_queue<T, Container>& pq) {
    auto& c = sad::helper::get_internal_container(pq);
    print_field_value(os, c);
}

// std::unordered_map / std::unorederd_multimap
template <typename CharT,
          typename Traits,
          template <typename, typename, typename, typename, typename> class UMap,
          typename Key,
          typename T,
          typename Hash,
          typename KeyEqual,
          typename Allocator>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os,
                              const UMap<Key, T, Hash, KeyEqual, Allocator>& m) {
    auto first = true;
    os << "{";
    std::for_each(m.cbegin(), m.cend(), [&first, &os](const auto& e) {
        if (not first) { os << ", "; }
        else { first = false; }
        os << "(";
        print_field_value(os, e.first);
        os << ", ";
        print_field_value(os, e.second);
        os << ")";
    });
    os << "}";
}


// std::unordered_map / std::unorederd_multimap
template <typename CharT,
          typename Traits,
          template <typename, typename, typename, typename> class Map,
          typename Key,
          typename T,
          typename KeyEqual,
          typename Allocator>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os,
                              const Map<Key, T, KeyEqual, Allocator>& m) {
    auto first = true;
    os << "{";
    std::for_each(m.cbegin(), m.cend(), [&first, &os](const auto& e) {
        if (not first) { os << ", "; }
        else { first = false; }
        os << "(";
        print_field_value(os, e.first);
        os << ", ";
        print_field_value(os, e.second);
        os << ")";
    });
    os << "}";
}

// std::set / std::multset
template <typename CharT,
          typename Traits,
          template <typename, typename, typename> class Set,
          typename Key,
          typename Compare,
          typename Allocator>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os,
                              const Set<Key, Compare, Allocator>& s) {
    auto first = true;
    os << "{";
    std::for_each(s.cbegin(), s.cend(), [&first, &os](const auto& e) {
        if (not first) { os << ", "; }
        else { first = false; }
        print_field_value(os, e);
    });
    os << "}";
}

template<typename CharT, typename Traits, typename... Types>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os,
                              const sad::schema_mapper<Types...>& schema) {
    auto first = true;
    os << "{";
    schema.for_each([&first, &os](const auto& f) {
        if (not first) { os << ", "; }
        else { first = false; }
        os << "'" << f.name << "': ";
        detail::basic_ostream::print_field_value(os, f.value);
    });
    os << "}";
}

}}

template <typename CharT, typename Traits = std::char_traits<CharT>>
struct basic_ostream_serializer {
private:
    std::basic_ostream<CharT, Traits>& out;
    bool pretty_ = false;

public:
    using serialized_type = std::basic_ostream<CharT, Traits>&;

    basic_ostream_serializer() = delete;
    basic_ostream_serializer(std::basic_ostream<CharT, Traits>& os)
    : out(os) {}
    ~basic_ostream_serializer() = default;

    basic_ostream_serializer& pretty(bool pretty_ = true) {
        this->pretty_ = pretty_;
        return *this;
    }

    template <typename T>
    std::basic_ostream<CharT, Traits>& serialize(const T& value) {
        detail::basic_ostream::print_field_value(this->out, sad::schema<T>()(value));
        return this->out;
    }

};

// basic serializers
using ostream_serializer = basic_ostream_serializer<char>;
using wostream_serializer = basic_ostream_serializer<wchar_t>;

// same as std::cout
static ostream_serializer cout_serializer{std::cout};

}}

#endif