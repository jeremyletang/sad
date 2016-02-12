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

#ifndef __SAD__TYPE_TRAITS__20160209__
#define __SAD__TYPE_TRAITS__20160209__

#include <type_traits>
#include "maybe_null.hpp"

namespace sad {

template <typename... Tys>
struct schema_mapper;

namespace traits {
namespace detail {

template <bool B>
using bool_constant = std::integral_constant<bool, B>;

struct void_t {};

template <template <typename> class Trait, typename T, typename... Rest>
struct ensure_trait_for_all :
    bool_constant<Trait<T>::value &&
                  detail::ensure_trait_for_all<Trait, Rest...>::value>
{};

template <template <typename> class Trait, typename T>
struct ensure_trait_for_all<Trait, T, void_t> : 
    bool_constant<Trait<T>::value>
{};

}

// is bool

template <typename T>
struct is_bool : std::false_type {};

template <>
struct is_bool<bool> : std::true_type {};

template <typename T>
constexpr bool is_bool_v = is_bool<T>::value;

// ensure for_all

template <template <typename> class Trait, typename... T>
struct ensure_trait_for_all :
    detail::bool_constant<detail::ensure_trait_for_all<Trait, T..., detail::void_t>::value>
{};

template <template <typename> class Trait, typename ... T>
constexpr bool ensure_trait_for_all_v = ensure_trait_for_all<Trait, T...>::value;

// maybe_null check

template <typename T>
struct is_maybe_null : std::false_type {};

template <typename T>
struct is_maybe_null<maybe_null<T>> : std::true_type {
    using inner_type = T;
};

template <typename T>
constexpr bool is_maybe_null_v = is_maybe_null<T>::value;

// schema_mapper check

template <typename T>
struct is_schema_mapper : std::false_type {};

template <typename... Tys>
struct is_schema_mapper<sad::schema_mapper<Tys...>> : std::true_type {};

template <typename T>
constexpr bool is_schema_mapper_v = is_schema_mapper<T>::value;

}}

#endif // __SAD__TYPE_TRAITS__20160209__