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

#ifndef __SAD__DESERIALIZE__20160213__
#define __SAD__DESERIALIZE__20160213__

#include <type_traits>
#include "utility.hpp"
#include "type_traits.hpp"

namespace sad {

template <typename OutValue>
struct deserialize_result {
    bool ok;
    std::string error_msg;
    OutValue value;
    deserialize_result(OutValue value)
    : ok(true), error_msg(std::string{""}), value(value) {}
    deserialize_result(const std::string& error_msg, OutValue value)
    : ok(false), error_msg(error_msg), value(value) {}

    bool is_ok() const { return this->ok; }
    bool operator!() const { return ok == false; }
    OutValue& operator*() { return this->value; }
    const OutValue& operator*() const { return this->value; }
};

template <typename OutValue, typename InValue, typename Deserializer>
deserialize_result<OutValue&> deserialize(Deserializer&& s, InValue& iv, OutValue& ov) {
    auto r = s.deserialize(iv, ov);
    if (not r.first) {
        return deserialize_result<OutValue&>{r.second, ov};
    }
    return deserialize_result<OutValue&>{ov};
}

template <typename OutValue, typename InValue, typename Deserializer>
deserialize_result<OutValue> deserialize(Deserializer&& s, InValue& iv) {
    auto ov = OutValue{};
    auto r = s.deserialize(iv, ov);
    if (not r.first) {
        return deserialize_result<OutValue>{r.second, ov};
    }
    return deserialize_result<OutValue>{ov};
}

}

#endif