## SAD

*SAD* is an header only, non intrusive serialization library for C++.
It should allow you to serialize your C++ struct / class (models), without having to implement any 
interface, just using function template specialization.

The library is designed to work with multiple backend, for now I'm working on a simple std::basic_ostream and a jsoncpp backend

*SAD* is a work in progress and mostly just build for the fun of doing templates, you should not use it in a real project.

## A simple Example

```C++
#include <string>
#include <sad/schema.hpp>
#include <sad/ostream.hpp>
#include <sad/utility.hpp>
#include <sad/backend/basic_ostream_serializer.hpp>

// define your model
stuct person {
    std::string name;
    unsigned int age;
    person() = default;
    person(const std::string name, unsigned int age)
    : name(name), age(age) {}
};

// specialize the sad::schema function for you model
namespace sad {
template <>
inline decltype(auto) schema(person& p) {
    return sad::make_schema(
        sad::f("name", p.name),
        sad::f("age", p.age)
    );
}}

int main() {
    // instanciate our model
    const auto p = person{"balek", 25};
    // serialize using the basic_ostream serializer
    // initialized with std::cout
    sad::serialize(sad::backend::basic_ostream{std::cout}, h);
}

```

## Installation

As specified earlier, *SAD* is an header only library, you just need to clone the repository, 
and copy everthing from the include folder inside your project.

You will only need to install locally the different libraries used internally depending of the backend you use.

## API Reference

TODO

## Tests

You can build the test suite using cmake to generate a build system for you plateform.

CMake will first download the required libraries to build the tests (googletest, and the backends dependencies).
You can specifies to build the test using this kind of command line:
```Shell
> mkdir build && cd build
> cmake -DBUILD_TESTS=ON -DWITH_JSONCPP_BACKEND=ON ..
> make
```
The first argument specify that we want to build the tests, the second one that we enable the 
jsoncpp backend.

## License

*SAD* is licensed under the MIT license, you can find a copy of the license at the root of the repository.
