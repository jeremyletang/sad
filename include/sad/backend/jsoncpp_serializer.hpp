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

#ifndef __SAD__BACKEND_JSONCPP_SERIALIZER__20160213__
#define __SAD__BACKEND_JSONCPP_SERIALIZER__20160213__

#include <type_traits>
#include <json/json.h>

#include "../schema.hpp"
#include "../utility.hpp"

namespace sad {
namespace backend {
namespace detail {
namespace jsoncpp {

// strings
template <typename CharT, typename Traits, typename Allocator>
inline void serialize_field_value(Json::Value& root,
                                  const std::basic_string<CharT, Traits, Allocator>& s,
                                  const std::string& name = "");
// unsigned numbers
template <typename T,
          typename std::enable_if<std::is_integral<T>::value &&
                                  std::is_unsigned<T>::value>::type* = nullptr>
inline void serialize_field_value(Json::Value& root,
                                  const T& t,
                                  const std::string& name = "");
// signed numbers
template <typename T,
          typename std::enable_if<std::is_integral<T>::value &&
                                  std::is_signed<T>::value>::type* = nullptr>
inline void serialize_field_value(Json::Value& root,
                                  const T& t,
                                  const std::string& name = "");
// reals 
template <typename T,
          typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
inline void serialize_field_value(Json::Value& root,
                                  const T& t,
                                  const std::string& name = "");
// schema
template<typename... Types>
inline void serialize_field_value(Json::Value& root,
                                  const sad::schema_mapper<Types...>& schema);
// any object
template <typename T,
          typename std::enable_if<!std::is_arithmetic<T>::value &&
                                  !sad::traits::is_maybe_null_v<T> &&
                                  !sad::traits::has_iterator_v<T>>::type* = nullptr>
inline void serialize_field_value(Json::Value& root,
                                  const T& t,
                                  const std::string& name = "");
// containers (std::list, std::vector, std::deque, std::forward_list ...)
template <template<typename, typename> class C,
          typename T,
          typename Allocator = std::allocator<T>>
inline void serialize_field_value(Json::Value& root,
                                  const C<T, Allocator>& t,
                                  const std::string& name = "");

// strings
template <typename CharT, typename Traits, typename Allocator>
inline void serialize_field_value(Json::Value& root,
                                  const std::basic_string<CharT, Traits, Allocator>& s,
                                  const std::string& name) {
    auto value = Json::Value(s);
    if (name.empty()) {root = value;} 
    else {root[name] = value;}
}

// unsigned numbers
template <typename T,
          typename std::enable_if<std::is_integral<T>::value &&
                                  std::is_unsigned<T>::value>::type*>
inline void serialize_field_value(Json::Value& root,
                                  const T& t,
                                  const std::string& name) {
    auto value = Json::Value(static_cast<Json::UInt>(t));
    if (name.empty()) {root = value;}
    else {root[name] = value;}
}

// signed numbers
template <typename T,
          typename std::enable_if<std::is_integral<T>::value &&
                                  std::is_signed<T>::value>::type*>
inline void serialize_field_value(Json::Value& root,
                                  const T& t,
                                  const std::string& name) {
    auto value = Json::Value(static_cast<Json::Int>(t));
    if (name.empty()) {root = value;}
    else {root[name] = value;}
}

// reals 
template <typename T,
          typename std::enable_if<std::is_floating_point<T>::value>::type*>
inline void serialize_field_value(Json::Value& root,
                                  const T& t,
                                  const std::string& name)  {
    auto value = Json::Value(static_cast<double>(t));
    if (name.empty()) {root = value;}
    else {root[name] = value;}
}

// schema
template<typename... Types>
inline void serialize_field_value(Json::Value& root,
                                  const sad::schema_mapper<Types...>& schema) {
    schema.for_each([&root](const auto& f) {
        detail::jsoncpp::serialize_field_value(root, f.value, f.name);
    });
}

// any object
template <typename T,
          typename std::enable_if<!std::is_arithmetic<T>::value &&
                                  !sad::traits::is_maybe_null_v<T> &&
                                  !sad::traits::has_iterator_v<T>>::type*>
inline void serialize_field_value(Json::Value& root,
                            const T& t,
                            const std::string& name) {
    auto value = Json::Value(Json::objectValue);
    if (name.empty()) {
        root = value;
        serialize_field_value(root, sad::schema<T>()(t));
    } else {
        root[name] = value;
        serialize_field_value(root[name], sad::schema<T>()(t));
    }
}

// containers (std::list, std::vector, std::deque, std::forward_list ...)
template <template<typename, typename> class C, typename T, typename Allocator>
inline void serialize_field_value(Json::Value& root,
                                  const C<T, Allocator>& t,
                                  const std::string& name) {
    auto value = Json::Value(Json::arrayValue);
    if (name.empty()) {root = value;}
    else {root[name] = value;}
    auto i = 0;
    for (const auto& v : t) {
        auto value_to_insert = Json::Value();
        serialize_field_value(value_to_insert, v);
        if (name.empty()) {root[i] = value_to_insert;}
        else {root[name][i] = value_to_insert;}
      i+=1;
    }
}

}}

struct jsoncpp_serializer {
private:
    Json::Value root;

public:
    using serialized_type = Json::Value;

    jsoncpp_serializer() = default;
    ~jsoncpp_serializer() = default;

    template <typename T>
    Json::Value serialize(const T& value) {
        sad::backend::detail::jsoncpp::serialize_field_value(this->root, sad::schema<T>()(value));
        return this->root;
    }

};


}}

#endif