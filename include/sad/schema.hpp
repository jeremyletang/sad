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

#ifndef __SAD__SCHEMA__20160209__
#define __SAD__SCHEMA__20160209__

#include <tuple>                // std::tuple
#include <string>               // std::string
#include <vector>               // std::vector
#include <functional>           // std::reference_wrapper
#include <stdexcept>            // std::invalid_argument
#include "tuple_utils.hpp"      // sad::tuple_utils::for_each
#include "type_traits.hpp"      // sad::traits::ensure_trait_for_all_v

namespace sad {

template <typename Value>
struct field {
    std::string name;
    Value& value;
    std::vector<std::string> tags;

    field() = delete;
    field(const std::string& name,
          Value& value,
          std::vector<std::string> tags = {})
    : name(name)
    , value(value)
    , tags(tags) {}
    ~field() = default;
};

template <typename T, typename U>
bool bind_value_ref_to_reference_wrapper(std::reference_wrapper<T>& t, U& val) { return false; }

template <typename T>
bool bind_value_ref_to_reference_wrapper(std::reference_wrapper<T>& t, T& val) {
    t = val;
    return true;
}


template <typename... Types>
struct schema_mapper {
    static_assert(
        sad::traits::ensure_trait_for_all_v<
            std::is_default_constructible,
            Types...
        >,
        "schema_mapper Types... must be default constructible.");

    std::tuple<field<Types>...> fields;

    schema_mapper() = delete;
    schema_mapper(field<Types>... fields)
    : fields(fields...) {}
    ~schema_mapper() = default;

    template <typename Fn>
    void for_each(Fn f) const {
        sad::tuple_utils::for_each(this->fields, f);
    }

    template <typename Fn>
    void for_each(Fn f) {
        sad::tuple_utils::for_each(this->fields, f);
    }

    template <typename T>
    T& get(const std::string& name) {
        T t{};
        auto ok = false;
        std::reference_wrapper<T> ref = std::ref(t);
        auto f = [&ok, &ref, &name](auto& f) {
            if (f.name == name) {
                ok |= bind_value_ref_to_reference_wrapper(ref, f.value);
            }
        };
        this->for_each(f);
        if (not ok) throw std::invalid_argument{"type or name not in schema"};
        return ref;
    }

    template <typename T>
    const T& get(const std::string& name) const {
        return const_cast<schema_mapper&>(*this).get<T>(name);
    }

    template <typename T>
    bool exist_with_type(const std::string& name) const {
        auto e = false;
        auto f = [&e, &name](auto& f) {
            if (f.name == name && std::is_same<T, decltype(f.value)>::value == true)
            { e = true; }
        };
        return e;
    }

    bool exist(const std::string& name) const {
        auto e = false;
        auto f = [&e, &name](auto& f) {
            if (f.name == name) { e = true; }
        };
        return e;
    }

};

}

#endif
