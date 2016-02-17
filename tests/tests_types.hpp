#include <list>
#include <type_traits>
#include <utility>
#include <array>
#include <map>
#include <unordered_map>
#include <set>
#include <sad/schema.hpp>
#include <sad/ostream.hpp>
#include <sad/utility.hpp>
#include <sad/maybe_null.hpp>

struct small {
    int integer;
    std::vector<std::vector<std::vector<std::vector<std::vector<std::vector<std::vector<int>>>>>>> vec;
    small() {}
    small(int integer)
    : integer(integer)
    , vec({{{{{{{1}}}}}}}) {}
};

struct inner {
    int blah;
    float bleh;
    std::vector<int> vec;
    std::array<small, 2> small_object;
    inner() = default;
    inner(int blah, float bleh, std::vector<int> vec = {}, std::array<small, 2> small_object = {})
    : blah(blah), bleh(bleh), vec(vec), small_object(small_object) {}
};

class hello {
    char c;
    std::string s;
    int i;
    inner in;
    float f;
    bool b;
    std::list<std::list<int>> l;
    std::pair<int, std::string> p;
    std::list<std::pair<int, std::string>> lp;
    std::tuple<int, std::string, float, std::pair<std::list<int>, bool>> t;
    sad::maybe_null<int> mi;
    sad::maybe_null<std::string> ms;
    std::array<float, 4> a;
    std::stack<double> sd;
    std::queue<double> q;
    std::priority_queue<double> pq;
    std::unordered_map<std::string, int> um;
    std::map<std::string, int> m;
    std::set<char> set_c;
public:
    hello() = default;
    hello(char c, std::string s, int i, inner in, float f, bool b, std::list<std::list<int>> l,
          std::string ms, std::array<float, 4> a, std::unordered_map<std::string, int> um,
          std::map<std::string, int> m, std::set<char> set_c)
    : c(c), s(s), i(i), in(in), f(f), b(b), l(l), ms(ms), a(a), um(um), m(m), set_c(set_c) {}

    friend struct sad::schema<hello>;
};



namespace sad {
template <>
struct schema<small> : public sad::base_schema<small>{
    using sad::base_schema<small>::operator();
    decltype(auto) operator()(small& s) {
        return sad::make_schema(
            sad::f("integer", s.integer),
            sad::f("vec", s.vec)
    );
    }
};
template <>
struct schema<inner> : public sad::base_schema<inner>{
    using sad::base_schema<inner>::operator();
    decltype(auto) operator()(inner& in) {
        return sad::make_schema(
            sad::f("blah", in.blah),
            sad::f("bleh", in.bleh),
            sad::f("vec", in.vec),
            sad::f("small_object", in.small_object)
    );
    }
};
template <>
struct schema<hello> : public sad::base_schema<hello>{
    using sad::base_schema<hello>::operator();
    decltype(auto) operator()(hello& h) {
        return sad::make_schema(
            sad::f("c", h.c),
            sad::f("s", h.s),
            sad::f("i", h.i),
            sad::f("in", h.in),
            sad::f("f", h.f),
            sad::f("b", h.b),
            sad::f("l", h.l),
            sad::f("lp", h.lp),
            sad::f("t", h.t),
            sad::f("mi", h.mi),
            sad::f("ms", h.ms),
            sad::f("a", h.a),
            sad::f("sd", h.sd),
            sad::f("q", h.q),
            sad::f("pq", h.pq),
            sad::f("um", h.um),
            sad::f("m", h.m),
            sad::f("set_c", h.set_c)
        );
    }
};
}