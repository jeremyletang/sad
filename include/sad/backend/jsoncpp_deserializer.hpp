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

#ifndef __SAD__BACKEND_JSONCPP_DESERIALIZER__20160213__
#define __SAD__BACKEND_JSONCPP_DESERIALIZER__20160213__

#include <iostream>
#include <utility>
#include <stdexcept>

#include "../type_traits.hpp"

namespace sad {
namespace backend {
namespace detail {
namespace jsoncpp {

struct missing_json_value: public std::exception {
public:
    std::string err;
    missing_json_value() = default;
    explicit missing_json_value(const std::string& err)
    : err(err) {}
    explicit missing_json_value(const std::string& value_name, const std::string& value_type)
    : err(std::string{"missing json value "}+value_name+" of expected type "+value_type) {}
    virtual const char* what() const throw() {
        return this->err.c_str();
    }
};

// utility function to use the good root in deserialization function
inline Json::Value& get_named_value_or_root(Json::Value& root,
                                            const std::string& key_name = "",
                                            const std::string& type_name = "unknown") {
    // we just need to return the root
    if (key_name.empty()) {return root;}
    // we need a given member
    // first check if the member exist and throw if not
    // else return the member
    if (not root.isMember(key_name)) {
        throw missing_json_value{key_name, type_name};
    }
    return root[key_name] ;
}

// schema
template<typename T, typename... Types>
inline void deserialize_field_value(Json::Value& root,
                                    sad::schema_mapper<T, Types...>& schema);
// any object
template <typename T,
          typename std::enable_if<!std::is_arithmetic<T>::value &&
                                  !sad::traits::is_maybe_null_v<T> &&
                                  !sad::traits::has_iterator_v<T>>::type* = nullptr>
inline void deserialize_field_value(Json::Value& root, T& t, const std::string& name = "");
// unsigned numbers
template <typename T,
          typename std::enable_if<std::is_integral<T>::value &&
                                  std::is_unsigned<T>::value>::type* = nullptr>
inline void deserialize_field_value(Json::Value& root,
                                    T& t,
                                    const std::string& name = "");
// signed numbers
template <typename T,
          typename std::enable_if<std::is_integral<T>::value &&
                                  std::is_signed<T>::value>::type* = nullptr>
inline void deserialize_field_value(Json::Value& root,
                                    T& t,
                                    const std::string& name = "");
// reals 
template <typename T,
          typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
inline void deserialize_field_value(Json::Value& root,
                                    T& t,
                                    const std::string& name = "");
// strings
template <typename CharT, typename Traits, typename Allocator>
inline void deserialize_field_value(Json::Value& root,
                                    const std::basic_string<CharT, Traits, Allocator>& s,
                                    const std::string& name = "");

// unsigned numbers
template <typename T,
          typename std::enable_if<std::is_integral<T>::value &&
                                  std::is_unsigned<T>::value>::type*>
inline void deserialize_field_value(Json::Value& root, T& t, const std::string& name) {
    auto& r = get_named_value_or_root(root, name);
    // boolean handling
    if (sad::traits::is_bool_v<T>) {
        if (not r.isBool()) { throw missing_json_value{name, "unsigned int"}; }
        t = r.asBool();
    } else { // uint
        if (not r.isUInt()) { throw missing_json_value{name, "unsigned int"}; }
        t = r.asUInt();
    }
}

// signed numbers
template <typename T,
          typename std::enable_if<std::is_integral<T>::value &&
                                  std::is_signed<T>::value>::type*>
inline void deserialize_field_value(Json::Value& root, T& t, const std::string& name) {
    auto& r = get_named_value_or_root(root, name);
    if (not r.isInt()) { throw missing_json_value{name, "int"}; }
    t = r.asInt();
}

// reals 
template <typename T,
          typename std::enable_if<std::is_floating_point<T>::value>::type*>
inline void deserialize_field_value(Json::Value& root, T& t, const std::string& name) {
    auto& r = get_named_value_or_root(root, name);
    if (not r.isDouble()) { throw missing_json_value{name, "double"}; }
    t = r.asDouble();
}

// strings
template <typename CharT, typename Traits, typename Allocator>
inline void deserialize_field_value(Json::Value& root,
                                    const std::basic_string<CharT, Traits, Allocator>& s,
                                    const std::string& name) {
    auto& r = get_named_value_or_root(root, name);
    if (not r.isString()) { throw missing_json_value{name, "string"}; }
    s = r.asString();
}

// schema
template<typename T, typename... Types>
inline void deserialize_field_value(Json::Value& root,
                                    sad::schema_mapper<T, Types...>& schema) {
    schema.for_each([&root](auto& f) {
        detail::jsoncpp::deserialize_field_value(root, f.value, f.name);
    });
}

// any object
template <typename T,
          typename std::enable_if<!std::is_arithmetic<T>::value &&
                                  !sad::traits::is_maybe_null_v<T> &&
                                  !sad::traits::has_iterator_v<T>>::type*>
inline void deserialize_field_value(Json::Value& root, T& t, const std::string& name) {
    auto s = sad::schema<T>()(t);
    auto& r = get_named_value_or_root(root, name);
    if (not r.isObject()) { throw missing_json_value{name, "object"}; }
    sad::backend::detail::jsoncpp::deserialize_field_value(root, s);
}

}}

struct jsoncpp_deserializer {
    jsoncpp_deserializer() = default;
    ~jsoncpp_deserializer() = default;

    // from string deserialization
    template <typename OutValue>
    std::pair<bool, std::string> deserialize(const std::string& input, OutValue& output) {
        auto reader = Json::Reader{};
        auto parsing_ok = reader.parse(input, this->root);
        if (not parsing_ok) {
            return std::make_pair(false, reader.getFormattedErrorMessages());
        }
        return this->deserialize(output);
    }

    // from filename deserialization
    template <typename OutValue>
    std::pair<bool, std::string> deserialize(std::istream& input, OutValue& output) {
        auto reader = Json::Reader{};
        auto parsing_ok = reader.parse(input, this->root);
        if (not parsing_ok) {
            return std::make_pair(false, reader.getFormattedErrorMessages());
        }
        return this->deserialize(output);
    }

private:
    Json::Value root;

    template <typename OutValue>
    std::pair<bool, std::string> deserialize(OutValue& output) {
        try {
            sad::backend::detail::jsoncpp::deserialize_field_value(this->root, output);
        } catch (const sad::backend::detail::jsoncpp::missing_json_value& e) {
            return std::make_pair(false, e.what());
        }
        return std::make_pair(true, std::string{});
    }
};

}}

#endif