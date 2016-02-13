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

#ifndef __SAD__CONTAINER_ADAPTER_HELPER__20160209__
#define __SAD__CONTAINER_ADAPTER_HELPER__20160209__

namespace sad {
namespace helper {

// container based on container without iterators helpers
// retrieve the container inside an std::queue / std::stack / std::priority_queue
template <typename T>
struct container_adapter_wrapper : public T {
    using container_type = typename T::container_type;
    const typename T::container_type& get_container() const {
        return this->c;
    }
};

template <typename T>
const typename T::container_type& get_internal_container(const T& q) {
    const auto* qw = reinterpret_cast<const container_adapter_wrapper<T>*>(&q);
    return qw->get_container();
}

}}

#endif // __SAD__CONTAINER_ADAPTER_HELPER__20160209__