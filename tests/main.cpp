#include <sstream>
#include "tests_types.hpp"
#include <sad/schema.hpp>
#include <sad/ostream.hpp>
#include <sad/maybe_null.hpp>
#include <sad/serialize.hpp>
#include <sad/deserialize.hpp>
#include <sad/backend/ostream_serializer.hpp>
#include <sad/backend/jsoncpp_serializer.hpp>
#include <sad/backend/jsoncpp_deserializer.hpp>
#include <gtest/gtest.h>
#include <json/json.h>

template <typename T>
struct ty;

void fun(const hello& h) {
    std::cout << sad::schema<hello>()(h) << std::endl;
}
int main () {
    const auto h = hello{
        '?',
        "hello world",
        42,
        {21, 12.12, {1, 2, 3, 4, 5}, {42, 84}},
        84.42,
        false,
        {{1, 2, 3}, {3,2,1}},
        "hello world",
        {42.2, 51.7, 0.987, 12.97},
        {{"pauline", 25}, {"jeremy", 25}, {"alexie", 22}},
        {{"pauline", 25}, {"jeremy", 25}, {"alexie", 22}},
        {'a', 'b', 'c', 'd'},
    };

    const auto i = inner{21, 12.12, {1, 2, 3, 4, 5}, {42, 84}};
    
    const auto _s = small{42};
    const auto _s_equal = small{42};
    const auto _s_not_equal = small{43};


    std::cout << "s == s_equal " << std::boolalpha << sad::equals(_s, _s_equal) << std::endl; 
    std::cout << "s != s_not_equal " << std::boolalpha << sad::equals(_s, _s_not_equal) << std::endl; 

    // fun(h);
    auto s = sad::schema<hello>()(h);
    std::cout << sad::schema<hello>().type_name() << std::endl;

    s.get<int>("i") = 250;
    // std::cout << "hello.i:" << s.get<int>("i") << std::endl;
    auto mn = std::move(sad::maybe_null<int>{});
    sad::maybe_null<int> mi = 42;
    // std::cout << mi << std::endl;

    sad::serialize(sad::backend::cout_serializer, h);
    std::cout << std::endl;
    auto json_value = sad::serialize(sad::backend::jsoncpp_serializer{}, i);
    std::cout << json_value.toStyledString() << std::endl;
    
    auto d_result =
        sad::deserialize<numbers>(sad::backend::jsoncpp_deserializer{},
                                  "{\"i\": 42, \"f\": 84.48, \"b\": true}");
    if (not d_result) {
        std::cout << d_result.error_msg << std::endl;
    }
    std::cout << "deserialized numbers: " << sad::schema<numbers>()(*d_result) << std::endl;

    // ty<decltype(s)> t;
}