#include <sstream>
#include "tests_types.hpp"
#include <sad/schema.hpp>
#include <sad/ostream.hpp>
#include <sad/maybe_null.hpp>
#include <sad/serialize.hpp>
#include <sad/backend/ostream_serializer.hpp>
#include <gtest/gtest.h>
#include <json/json.h>

template <typename T>
struct ty;

void fun(const hello& h) {
    std::cout << sad::schema(h) << std::endl;
}

int main () {
    const auto h = hello{
        '?',
        "hello world",
        42,
        {21, 12.12},
        84.42,
        false,
        {{1, 2, 3}, {3,2,1}},
        "hello world",
        {42.2, 51.7, 0.987, 12.97},
        {{"pauline", 25}, {"jeremy", 25}, {"alexie", 22}},
        {{"pauline", 25}, {"jeremy", 25}, {"alexie", 22}},
        {'a', 'b', 'c', 'd'},
    };

    // fun(h);
    auto s = sad::schema(h);
    s.get<int>("i") = 250;
    // std::cout << "hello.i:" << s.get<int>("i") << std::endl;
    auto mn = std::move(sad::maybe_null<int>{});
    sad::maybe_null<int> mi = 42;
    // std::cout << mi << std::endl;

    sad::serialize(sad::backend::cout_serializer.pretty(), h);
    // sad::serialize(sad::backend::ostream_serializer{std::cout}, h);
    
    // ty<decltype(s)> t;
}