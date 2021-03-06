#include <imap.hpp>

#include "helpers.hpp"

#include <vector>
#include <string>
#include <iterator>

#include "catch.hpp"

using iter::imap;
using Vec = const std::vector<int>;

namespace {
    int plusone(int i) {
        return i + 1;
    }

    class PlusOner {
        public:
            int operator()(int i) {
                return i + 1;
            }
    };

    int power(int b, int e) {
        int acc = 1;
        for (int i = 0; i < e; ++i) {
            acc *= b;
        }
        return acc;
    }
}

TEST_CASE("imap: works with lambda, callable, and function", "[imap]") {
    Vec ns = {10, 20, 30};
    SECTION("with lambda") {
        auto im = imap([](int i) { return i + 1; }, ns);
        Vec v(std::begin(im), std::end(im));
        Vec vc = {11, 21, 31};
        REQUIRE( v == vc );
    }

    SECTION("with function") {
        auto im = imap(plusone, ns);
        Vec v(std::begin(im), std::end(im));
        Vec vc = {11, 21, 31};
        REQUIRE( v == vc );
    }

    SECTION("with function") {
        auto im = imap(PlusOner{}, ns);
        Vec v(std::begin(im), std::end(im));
        Vec vc = {11, 21, 31};
        REQUIRE( v == vc );
    }

}

TEST_CASE("imap: works with multiple sequences", "[imap]") {
    Vec bases = {0, 1, 2, 3};
    Vec exps = {1, 2, 3, 4};

    auto im = imap(power, bases, exps);
    Vec v(std::begin(im), std::end(im));
    Vec vc = {0, 1, 8, 81};

    REQUIRE( v == vc );
}

TEST_CASE("imap: terminates on shortest squence", "[imap]") {
    Vec ns1 = {1, 2, 3, 4};
    Vec ns2 = {2, 4, 6, 8, 10};
    Vec vc = {3, 6, 9, 12};
    SECTION("shortest sequence first") {
        auto im = imap([](int a, int b){ return a + b; }, ns1, ns2);
        Vec v(std::begin(im), std::end(im));
        REQUIRE( v == vc );
    }
    SECTION("shortest sequence second") {
        auto im = imap([](int a, int b){ return a + b; }, ns2, ns1);
        Vec v(std::begin(im), std::end(im));
        REQUIRE( v == vc );
    }
}

TEST_CASE("imap: empty sequence gives nothing", "[imap]") {
    Vec v{};
    auto im = imap(plusone, v);
    REQUIRE( std::begin(im) == std::end(im) );
}

TEST_CASE("imap: binds to lvalues, moves rvalues", "[imap]") {
    itertest::BasicIterable<int> bi{1, 2};
    SECTION("binds to lvalues") {
        imap(plusone, bi);
        REQUIRE_FALSE(bi.was_moved_from());
    }

    SECTION("moves rvalues") {
        imap(plusone, std::move(bi));
        REQUIRE(bi.was_moved_from());
    }
}

TEST_CASE("imap: doesn't move or copy elements of iterable", "[imap]") {
    constexpr itertest::SolidInt arr[] = {{1}, {0}, {2}};
    for (auto&& i : imap([](const itertest::SolidInt& si){return si.getint();},
                arr)) {
        (void)i;
    }
}

TEST_CASE("imap: postfix ++", "[imap]") {
    Vec ns = {10, 20};
    auto im = imap(plusone, ns);
    auto it = std::begin(im);
    it++;
    REQUIRE( (*it) == 21 );
    it++;
    REQUIRE( it == std::end(im) );
}

TEST_CASE("imap: iterator meets requirements", "[imap]") {
    std::string s{};
    auto c = imap([](char){return 1;}, s);
    REQUIRE( itertest::IsIterator<decltype(std::begin(c))>::value );
}
