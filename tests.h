#ifndef GROBNER_TESTS_H_
#define GROBNER_TESTS_H_

#include <boost/rational.hpp>
#include <cassert>
#include <iostream>
#include <sstream>

#include "algorithm.h"
#include "modular.h"
#include "monomial.h"
#include "order.h"
#include "polynomial.h"
#include "polynomial_set.h"

namespace grobner {

class Tests {
  public:
    static void test_all();

    static void test_modular();
    static void test_monomial();
    static void test_polynomial();
    static void test_polynomial_set();
    static void test_monomial_order();
    static void test_algorithm();
 
  private:
    static void test_rational_polynomial();
    static void test_z_p_polynomial();
};

void Tests::test_all() {
    test_modular();
    test_monomial();
    test_polynomial();
    test_polynomial_set();
    test_monomial_order();
    test_algorithm();
}

void Tests::test_modular() {
    using Modular = Modular<17ll>;

    assert(Modular(2) == Modular(-15));
    assert(Modular(36) == Modular(-15));
    assert(Modular(1) != Modular(-1));
    assert(Modular(2) != Modular(-14));

    assert(Modular(13) + Modular(12) == Modular(8));
    assert(Modular(34) - Modular(3) + Modular(5) == Modular(2));
    assert(-Modular(5) == Modular(-5));

    assert(Modular(1958) * Modular(-16) == Modular(1958));
    assert(Modular(2) * Modular(3) == Modular(6));
    assert(Modular(5) * Modular(7) == Modular(1));
    assert(Modular(5).get_inverse() == Modular(7));
    assert(Modular(2) / Modular(1) == Modular(2));
    assert(Modular(2) / Modular(7) == Modular(10));

    assert(Modular(2).get_pow(10) == Modular(1024));
    assert(Modular(1).get_pow(1000000000) == Modular(1));
    assert(Modular(12345).get_pow(1600000000) == Modular(1));
    assert(Modular(1549382).get_pow(17) == Modular(1549382));
    assert(Modular(12345).get_pow(-1) == Modular(12345).get_inverse());
    assert(Modular(12345).get_pow(0) == Modular(1));
    assert(Modular(56789).get_pow(-123) == Modular(56789).get_pow(123).get_inverse());

    assert(Modular(35).get_value() == 1);

    std::stringstream ss;
    ss << 36 << ' ' << -8;
    Modular m1, m2;
    ss >> m1 >> m2;
    assert(m1 == Modular(2));
    assert(m2 == Modular(9));

    std::cout << "Modular tests passed" << std::endl;
}

void Tests::test_monomial() {
    assert(Monomial() == Monomial({0, 0, 0}));
    assert(Monomial() == Monomial({}));
    assert(Monomial() == Monomial(""));
    assert(Monomial() == Monomial("ab^0c^-1ca^2a^-3"));
    assert(Monomial("a^2d^4c^1c^-1f^0") == Monomial({2, 0, 0, 4}));
    assert(Monomial("a") != Monomial({0, 1}));
    assert(Monomial("abc") == Monomial("cba"));

    Monomial x("ab^2c"), y("bc"), z("ac");
    assert(x * y == Monomial({1, 3, 2}));
    assert(y * z == Monomial("abcc"));

    assert(x.is_divisible_by(y));
    assert(x.is_divisible_by(z));
    assert(!x.is_divisible_by(y * z));

    assert(x / y == Monomial("ab"));
    assert(x / z == Monomial("b") * Monomial("b"));

    assert(Monomial("aba") * Monomial("a^-2b^-1") == Monomial());

    Monomial m1, m2;
    m1.set_degree(2019, 5);
    m1.set_degree(5, 3);
    m2.set_degree(2019, -5);
    m1 *= m2;
    assert(m1 == Monomial({0, 0, 0, 0, 0, 3}));

    std::stringstream ss;
    m1 = Monomial({1, 0, 3, -2});
    ss << m1;
    assert(ss.str() == "ac^3d^-2");
    m2.read(ss);
    assert(m1 == m2);

    std::cout << "Monomial tests passed" << std::endl;
}

void Tests::test_polynomial() {
    Tests::test_rational_polynomial();
    Tests::test_z_p_polynomial();
    std::cout << "Polynomial tests passed" << std::endl;
}

void Tests::test_polynomial_set() {
    using Value = Modular<17ll>;
    using Poly = Polynomial<Value>;
    using Set = PolynomialSet<Value>;

    assert(Set() == Set());
    assert(Set({Poly("1a"), Poly("2c+3d")}) == Set({Poly("2c+3d"), Poly("1a")}));

    Set s1, s2;
    s1.insert(Poly(Value(0)));
    assert(s1 != s2);
    s1.erase(Poly());
    assert(s1 == s2);
    s1.insert(Poly("1a+1b"));
    s1.insert(Poly("1a+1b"));
    s1.insert(Poly("1a+1b"));
    s1.insert(Poly("1a-1b"));
    assert(s1.size() == 2);
    s1.clear();
    assert(s1 == s2);
    assert(s1.empty());
    assert(s2.empty());

    s1.insert(Poly("1a+2b"));
    s1.insert(Poly("2a+2b"));
    s1.insert(Poly("2a^2+2b"));
    s1.insert(Poly("1b^100"));
    for (auto poly : s1)
        s2.insert(poly);
    assert(s1 == s2);

    int cnt = 0;
    for (auto it = s2.pbegin(); it != s2.pend(); ++it)
        ++cnt;
    assert(cnt == 6);

    std::cout << "PolynomialSet tests passed" << std::endl;
}

void Tests::test_monomial_order() {
    using Deg = DegOrder;
    using Lex = LexOrder;
    using DegLex = OrderSum<Deg, Lex>;

    Monomial a("a^2b"), b("b^100"), c("a"), d("abcdef"), e("c^3"), f("abc");

    assert(Deg::cmp(a, e) == 0);
    assert(Deg::cmp(a, b) == -1);
    assert(Deg::cmp(a, c) == 1);
    assert(Deg::cmp(d, c) == 1);
    assert(Deg::cmp(a, b) == -1);
    assert(Deg::cmp(a, f) == 0);

    assert(Lex::cmp(a, b) == 1);
    assert(Lex::cmp(a, c) == 1);
    assert(Lex::cmp(a, f) == 1);
    assert(Lex::cmp(c, d) == -1);
    assert(Lex::cmp(c, f) == -1);
    assert(Lex::cmp(d, f) == 1);
    assert(Lex::cmp(c, b) == 1);

    assert(DegLex::cmp(a, d) == -1);
    assert(DegLex::cmp(a, b) == -1);
    assert(DegLex::cmp(a, f) == 1);
    assert(DegLex::cmp(a, e) == 1);
    assert(DegLex::cmp(c, b) == -1);
    assert(DegLex::cmp(b, e) == 1);
    assert(DegLex::cmp(f, e) == 1);

    std::cout << "MonomialOrder tests passed" << std::endl;
}

void Tests::test_algorithm() {
    using Deg = DegOrder;
    using Lex = LexOrder;
    using DegLex = OrderSum<Deg, Lex>;

    using Value = Modular<17ll>;
    using Poly = Polynomial<Value>;
    using Set = PolynomialSet<Value>;

    assert(Algorithm::get_LCM(Monomial({0, 1, 2, 0, 0}), Monomial({5, 2, 0, 1})) == Monomial({5, 2, 2, 1}));
    assert(Algorithm::get_LCM(Monomial({5, 2, 0, 1}), Monomial({0, 1, 2, 0, 0})) == Monomial({5, 2, 2, 1}));
    assert(Algorithm::get_LCM(Monomial({123}), Monomial({234})) == Monomial("a^234"));

    assert(Algorithm::get_leading_monomial<Deg>   (Poly("5a^2bd^30+10a^2b^2c+3d^50")) ==
                                                                        std::make_pair(Monomial("d^50"), Value(3)));
    assert(Algorithm::get_leading_monomial<Lex>   (Poly("5a^2bd^30+10a^2b^2c+3d^50")) ==
                                                                        std::make_pair(Monomial("a^2b^2c"), Value(10)));
    assert(Algorithm::get_leading_monomial<DegLex>(Poly("5a^2bd^30+10a^2b^2c")) ==
                                                                        std::make_pair(Monomial("a^2bd^30"), Value(5)));

    Poly g("1abc"), f1("1ab-1d"), f2("1ac-1d"), f3("1bd-1cd");

    auto g_tmp = g;
    Algorithm::reduce_by<DegLex>(g, f1);
    assert(g == Poly("1cd"));

    g = g_tmp;
    Algorithm::reduce_by<DegLex>(g, f2);
    assert(g == Poly("1bd"));

    assert(Algorithm::get_S<DegLex>(f1, f2) == f3);

    Set F = {f1, f2};

    auto f3_tmp = f3;
    Algorithm::reduce_by<DegLex>(f3, F);
    assert(f3 != Poly());

    Algorithm::extend_to_grobners_basis<DegLex>(F);
    f3 = f3_tmp;
    Algorithm::reduce_by<DegLex>(f3, F);
    assert(f3 == Poly());

    for (auto it = F.pbegin(); it != F.pend(); ++it) {
        auto f1 = *((*it).first);
        auto f2 = *((*it).second);
        auto S = Algorithm::get_S<DegLex>(f1, f2);
        Algorithm::reduce_by<DegLex>(S, F);
        assert(S == Poly());
    }

    std::cout << "Algorithm tests passed" << std::endl;
}



void Tests::test_rational_polynomial() {
    using Value = boost::rational<long long>;
    using Poly = Polynomial<Value>;

    assert(Poly() == Poly(""));
    assert(Poly() == Poly({}));
    assert(Poly() == Poly(Value(0)));
    assert(Poly(Value(1)) == Poly(Monomial()));
    assert(Poly("1/1a") != Poly(Value(1)));
    assert(Poly("1/1a") != Poly("1/1b"));

    assert(Poly("1/1ab^2c") == Poly(Monomial("cbab")));
    assert(Poly("1/1ab+2/15bc-3/3ab-5/4") == Poly(Monomial("bc")) * Value(2, 15) - Poly(Value(5, 4)));
    assert(Poly("1/1a+1/1b") * Poly("1/1a-1/1b") == Poly("1/1a^2-1/1b^2"));
    assert(Poly("1/1a+1/1b") * Poly("1/1a+1/1b") == Poly("1/1a^2+1/1b^2+100/50ab"));

    assert(Poly("2/1a+3/4b^2") / Value(5) == Poly("2/5a+3/20b^2"));
    assert(Poly("2/1ab+3/4b^2") / Monomial("b") == Poly("2/1a+3/4b"));

    assert(Poly("2/1a+3/4b") - Poly("2/1a+3/4b") == Poly());
    assert(Poly("2/1a+3/4b") - Poly("3/4b") == Poly("2/1a"));
    assert(Poly("2/1a+3/4b") + Poly("-3/4b") == Poly("2/1a"));

    Poly p;
    p.add_monomial(Monomial("xyz"), Value(100));
    p.add_monomial(Monomial("ab"));
    p.add_monomial(Monomial("c^3"), Value(1, 2));
    p.add_monomial(Monomial("xyz"), Value(-100));
    assert(p == Poly("1/2c^3+1/1ab"));
}

void Tests::test_z_p_polynomial() {
    using Value = Modular<17ll>;
    using Poly = Polynomial<Value>;

    assert(Poly() == Poly(""));
    assert(Poly() == Poly({}));
    assert(Poly() == Poly(Value(0)));
    assert(Poly(Value(1)) == Poly(Monomial()));
    assert(Poly("1a") != Poly(Value(1)));
    assert(Poly("1a") != Poly("1b"));

    assert(Poly("1ab^2c") == Poly(Monomial("cbab")));
    assert(Poly("1ab+25bc-18ab-5") == Poly(Monomial("bc")) * Value(8) - Poly(Value(5)));
    assert(Poly("1a+1b") * Poly("1a+16b") == Poly("1a^2-1b^2"));
    assert(Poly("1a+1b") * Poly("1a+1b") == Poly("1a^2-16b^2+36ab"));

    assert(Poly("2a+3b^2") / Value(5) == Poly("14a+4b^2"));  // 5 * 7 == 1 mod 17
    assert(Poly("2ab+3b^2") / Monomial("b") == Poly("2a+3b"));

    assert(Poly("2a+3b") - Poly("2a+3b") == Poly());
    assert(Poly("2a+3b") - Poly("3b") == Poly("2a"));
    assert(Poly("2a+3b") + Poly("-3b") == Poly("2a"));

    Poly p;
    p.add_monomial(Monomial("xyz"), Value(100));
    p.add_monomial(Monomial("ab"));
    p.add_monomial(Monomial("c^3"), Value(2));
    p.add_monomial(Monomial("xyz"), Value(70));
    assert(p == Poly("2c^3+1ab"));

    using Poly2 = Polynomial<Modular<2>>;
    assert(Poly2("2b+2a+28g") == Poly2());
}

}  // grobner

#endif  // GROBNER_TESTS_H_
