#include <sad/schema.hpp>
#include <sad/ostream.hpp>
#include "tests_types.hpp"

template <typename T>
struct ty;

void fun(const hello& h) {
    std::cout << sad::schema(h) << std::endl;
}

int main () {
    const auto h = hello{
        "hello world",
        42,
        {21, 12.12},
        84.42,
        false,
        {{1, 2, 3}, {3,2,1}}
    };

    fun(h);
    auto s = sad::schema(h);
    s.get<int>("i") = 250;
    std::cout << "hello.i:" << s.get<int>("i") << std::endl;
    // ty<decltype(s)> t;
}