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
#include <cstdio>
#include "../utility.hpp"

namespace sad {
namespace backend {
namespace detail {

}

struct jsoncpp_deserializer {
    jsoncpp_deserializer() = default;
    ~jsoncpp_deserializer() = default;

    // from string deserialization
    template <typename OutValue>
    std::pair<bool, std::string> deserialize(const std::string& input, OutValue& output) {
        auto root = Json::Value();
        auto reader = Json::Reader();
        auto parsing_ok = reader.parse(input, root);
        if (not parsing_ok) {
            return std::make_pair(false, reader.getFormattedErrorMessages());
        }

        return std::make_pair(true, std::string{});
    }

    // from filename deserialization
    template <typename OutValue>
    std::pair<bool, std::string> deserialize(std::istream& input, OutValue& output) {
        auto root = Json::Value();
        auto reader = Json::Reader();
        auto parsing_ok = reader.parse(input, root);
        if (not parsing_ok) {
            return std::make_pair(false, reader.getFormattedErrorMessages());
        }

        return std::make_pair(true, std::string{});
    }

};

}}

#endif