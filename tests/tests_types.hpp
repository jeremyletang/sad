#include <list>
#include <type_traits>
#include <utility>
#include <sad/schema.hpp>
#include <sad/ostream.hpp>
#include <sad/utility.hpp>

struct inner {
    int blah;
    float bleh;
    inner() = default;
    inner(int blah, float bleh)
    : blah(blah), bleh(bleh) {}
};

class hello {
    std::string s;
    int i;
    inner in;
    float f;
    bool b;
    std::list<std::list<int>> l;
    std::pair<int, std::string> p;
    std::list<std::pair<int, std::string>> lp;
    std::tuple<int, std::string, float, std::pair<std::list<int>, bool>> t;
public:
    hello() = default;
    hello(std::string s, int i, inner in, float f, bool b, std::list<std::list<int>> l)
    : s(s), i(i), in(in), f(f), b(b), l(l) {}

    friend decltype(auto) sad::schema<>(hello& h);
};

namespace sad {
template <>
inline decltype(auto) schema(inner& in) {
    return sad::make_schema(
        sad::f("blah", in.blah),
        sad::f("bleh", in.bleh)
    );
}
template <>
inline decltype(auto) schema(hello& h) {
    return sad::make_schema(
        sad::f("s", h.s),
        sad::f("i", h.i),
        sad::f("in", h.in),
        sad::f("f", h.f),
        sad::f("b", h.b),
        sad::f("l", h.l),
        sad::f("lp", h.lp),
        sad::f("t", h.t)
    );
}
}
