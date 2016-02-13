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
#include "../ostream.hpp"

namespace sad {
namespace backend {

template <typename CharT, typename Traits = std::char_traits<CharT>>
struct basic_ostream_serializer {
    std::basic_ostream<CharT, Traits>& out;

    using serialized_type = std::basic_ostream<CharT, Traits>&;

    basic_ostream_serializer() = delete;
    basic_ostream_serializer(std::basic_ostream<CharT, Traits>& os)
    : out(os) {}
    ~basic_ostream_serializer() = default;

    template <typename T>
    std::basic_ostream<CharT, Traits>& serialize(const T& value) {
        this->out << sad::schema(value);
        return this->out;
    }

};

using ostream_serializer = basic_ostream_serializer<char>;
using wostream_serializer = basic_ostream_serializer<wchar_t>;

}}

#endif