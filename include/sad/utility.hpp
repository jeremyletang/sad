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
#include <unordered_map>
#include "schema.hpp"

namespace sad {

template <typename... Types>
decltype(auto) make_schema(sad::field<Types>... fields) {
    return sad::schema_mapper<Types...>(fields...);
}

template <typename MemberRef>
decltype(auto) f(const std::string& name,
                 MemberRef& member_ref,
                 std::unordered_map<std::string, std::string> tags = {}) {
    return sad::field<MemberRef>{name, member_ref, tags};
}

template <typename T>
decltype(auto) schema(T&);

template <typename T>
decltype(auto) schema(const T& t) {
    const auto s = schema(const_cast<T&>(t));
    return s;
}

}

#endif // __SAD__UTILITY__20160209__