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

#ifndef __SAD__UTILITY__20160209__
#define __SAD__UTILITY__20160209__

#include <string>
#include <vector>
#include <typeinfo>

#include "schema.hpp"
#include "demangle.hpp"

namespace sad {

template <typename T, typename... Types>
decltype(auto) make_schema(sad::field<Types>... fields) {
    return sad::schema_mapper<T, Types...>(fields...);
}

template <typename MemberRef>
decltype(auto) f(const std::string& name,
                 MemberRef& member_ref,
                 std::vector<std::string> tags = {}) {
    return sad::field<MemberRef>{name, member_ref, tags};
}

template <typename T>
struct schema;

template <typename T>
struct base_schema {
    virtual std::string type_name() const {
        return sad::demangle::type_name(typeid(T).name());
    }

    decltype(auto) operator()(const T& p) const {
        const auto s = schema<T>()(const_cast<T&>(p));
        return s;
    }
};

template <typename... Tys>
bool operator==(const schema_mapper<Tys...>& sm1, const schema_mapper<Tys...>& sm2) {
    auto ok = true;
    auto fn = [&sm2, &ok](const auto& f) {
        const auto sm2_value =
            sm2.template get<std::remove_reference_t<decltype(f.value)>>(f.name);
        if (sm2_value not_eq f.value) {
            ok = false;
        }
    };
    sm1.for_each(fn);
    return ok;
}

template <typename... Tys>
bool operator!=(const schema_mapper<Tys...>& sm1, const schema_mapper<Tys...>& sm2) {
    return not (sm1 == sm2);
}

template <typename T>
bool equals(const T& t1, const T& t2) {
    auto s1 = sad::schema<T>()(t1);
    auto s2 = sad::schema<T>()(t2);
    return s1 == s2;
}

}

#endif // __SAD__UTILITY__20160209__
