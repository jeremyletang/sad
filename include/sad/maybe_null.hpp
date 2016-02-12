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

#ifndef __SAD__MAYBE_NULL__201602101852__
#define __SAD__MAYBE_NULL__201602101852__

#include <type_traits>                  // std::is_default_constructible
#include <functional>                   // std::move
#include <stdexcept>                    // std::except

namespace sad {

template <typename T>
struct maybe_null {
    static_assert(std::is_default_constructible<T>::value,
                  "maybe_null type parameter must be default constructible");
private:
    bool null;
    T value;

public:
    maybe_null()
    : null(true)
    , value(T{}) {}

    maybe_null(T&& value)
    : null(false)
    , value(std::forward<T>(value)) {}

    maybe_null(const T& value)
    : null(false)
    , value(value) {}

    maybe_null(maybe_null&& oth)
    : null(oth.null)
    , value(std::move(oth.value)) {
        oth.null = true;
    }

    maybe_null& operator=(const maybe_null& oth) {
        if (*this != oth) {
            this->value = oth.value;
            this->null = oth.null;
        }
        return *this;
    }

    maybe_null& operator=(maybe_null&& oth) {
        if (*this != oth) {
            this->value = std::move(oth.value);
            this->null = oth.null;
            oth.null = true;
        }
        return *this;
    }

    maybe_null& operator=(T&& value) {
        this->value = std::forward<T>(value);
        this->null = false;
    }

    bool is_null() const {
        return this->null;
    }

    T&& unwrap() {
        if (this->null) {
            throw std::logic_error{"cannot unwrap maybe_null value if not set"};
        }
        this->null = true;
        return std::move(this->value);
    }

    operator const T&() const {
        if (this->null) {
            throw std::logic_error{"cannot access maybe_null value if not set"};
        }
        return this->value;
    }

    const T& operator*() const {
        if (this->null) {
            throw std::logic_error{"cannot access maybe_null value if not set"};
        }
        return this->value;
    }

    T& operator->() {
        if (this->null) {
            throw std::logic_error{"cannot access maybe_null value if not set"};
        }
        return this->value;
    }

    void set(T&& new_value) {
        this->value = std::forward<T>(new_value);
        this->null = false;
    }


    ~maybe_null() = default;

};

}

#endif // __SAD__MAYBE_NULL__201602101852__