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

#ifndef __SAD__OSTREAM__20160209__
#define __SAD__OSTREAM__20160209__

#include <iostream>
#include <algorithm>
#include <type_traits>
#include "schema.hpp"
#include "utility.hpp"
#include "tuple_utils.hpp"
#include "type_traits.hpp"
#include "maybe_null.hpp"

namespace sad {

// declarations

// std::basic_string
template <typename CharT, typename Traits, typename Allocator>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os,
                              const std::basic_string<CharT, Traits, Allocator>& s);
// numbers
template <typename CharT,
          typename Traits = std::char_traits<CharT>,
          typename T,
          typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os, const T& t);
// any object
template <typename CharT,
          typename Traits = std::char_traits<CharT>,
          typename T,
          typename std::enable_if<!std::is_arithmetic<T>::value &&
                                  !sad::traits::is_maybe_null_v<T>>::type* = nullptr>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os, const T& t);
// containers (std::list, std::vector, std::deque, std::forward_list ...)
template <typename CharT,
          typename Traits = std::char_traits<CharT>,
          typename T,
          template<typename Ty, typename Allocator = std::allocator<Ty>> class C>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os, const C<T>& t);
// std::pair
template <typename CharT,
          typename Traits = std::char_traits<CharT>,
          typename T1,
          typename T2>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os, const std::pair<T1, T2>& p);
// std::tuple
template <typename CharT,
          typename Traits = std::char_traits<CharT>,
          typename... Tys>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os, const std::tuple<Tys...>& t);
// sad::maybe_null
template <typename CharT,
          typename Traits = std::char_traits<CharT>,
          typename T>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os, const sad::maybe_null<T>& mt);

// definitions

// std::basic_string
template <typename CharT, typename Traits, typename Allocator>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os,
                              const std::basic_string<CharT, Traits, Allocator>& s) {
    os << "\"" << s << "\"";
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
                                  !sad::traits::is_maybe_null_v<T>>::type*>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os, const T& t) {
    os << sad::schema(t);
}

// containers
template <typename CharT,
          typename Traits,
          typename T,
          template<typename Ty, typename Allocator = std::allocator<Ty>> class C>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os, const C<T>& t) {
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
template <typename CharT,
          typename Traits,
          typename T1,
          typename T2>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os,
                              const std::pair<T1, T2>& p) {
    os << "{'first': ";
    print_field_value(os, p.first);
    os << ", 'second': ";
    print_field_value(os, p.second);
    os << "}";
}

// tuples
template <typename CharT,
          typename Traits,
          typename... Tys>
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
template <typename CharT,
          typename Traits,
          typename T>
inline void print_field_value(std::basic_ostream<CharT, Traits>& os,
                              const sad::maybe_null<T>& mt) {
    if (mt.is_null()) { os << "null"; } 
    else { print_field_value(os, *mt); }
}

template<typename CharT,
         typename Traits,
         typename... Types>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                              const sad::schema_mapper<Types...>& schema) {
    auto first = true;
    os << "{";
    schema.for_each([&first, &os](const auto& f) {
        if (not first) { os << ", "; }
        else { first = false; }
        os << "'" << f.name << "': ";
        print_field_value(os, f.value);
    });
    os << "}";
    return os;
}

template<typename CharT, typename Traits, typename U>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                              const sad::maybe_null<U>& t) {
    print_field_value(os, t);
    return os;
}

}

#endif // __SAD__OSTREAM__20160209__
