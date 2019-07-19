#ifndef GROBNER_SPEED_TESTS_H_
#define GROBNER_SPEED_TESTS_H_

#include <boost/rational.hpp>
#include <ctime>
#include <iostream>

#include "algorithm.h"
#include "modular.h"
#include "monomial.h"
#include "polynomial.h"
#include "polynomial_set.h"
#include "timer_wrapper.h"

namespace grobner {

class SpeedTests {
  public:
    inline static void run_tests();

    template<class ValueType>
    inline static void test_cyclic(int variable_count);

  private:
    template<class ValueType>
    inline static PolynomialSet<ValueType> get_cyclic(int variable_count);
    template<class ValueType>
    inline static Polynomial<ValueType> get_sigma(int variable_count, int sigma_index);
};


void SpeedTests::run_tests() {
    // using ValueType = boost::rational<long long>;
    using ValueType = Modular<(long long)1e9 + 7>;

    test_cyclic<ValueType>(1);
    test_cyclic<ValueType>(2);
    test_cyclic<ValueType>(3);
    test_cyclic<ValueType>(4);
    test_cyclic<ValueType>(5);
    test_cyclic<ValueType>(6);
    test_cyclic<ValueType>(7);
    test_cyclic<ValueType>(8);
}

template<class ValueType>
void SpeedTests::test_cyclic(int variable_count) {
    using Lex = LexOrder;
    using DegRevLex = OrderSum<DegOrder, RevLexOrder>;

    auto cyclic = get_cyclic<ValueType>(variable_count);

    clock_t elapsed_time = 0;
    test_time(Algorithm::extend_to_grobners_basis<DegRevLex, ValueType>, elapsed_time).call(&cyclic);
    test_time(Algorithm::extend_to_grobners_basis<Lex, ValueType>, elapsed_time).call(&cyclic);
    std::cout << "Cyclic(" << variable_count << ") with DegRevLex: "  <<
        (long double)elapsed_time / CLOCKS_PER_SEC << "s" << std::endl;

    elapsed_time = 0;
    cyclic = get_cyclic<ValueType>(variable_count);
    test_time(Algorithm::extend_to_grobners_basis<Lex, ValueType>, elapsed_time).call(&cyclic);
    std::cout << "Cyclic(" << variable_count << ") without DegRevLex: "  <<
        (long double)elapsed_time / CLOCKS_PER_SEC << "s" << std::endl;
}

template<class ValueType>
PolynomialSet<ValueType> SpeedTests::get_cyclic(int variable_count) {
    PolynomialSet<ValueType> result;
    for (int i = 1; i < variable_count; ++i) {
        result.insert(get_sigma<ValueType>(variable_count, i));
    }
    result.insert(get_sigma<ValueType>(variable_count, variable_count) - ValueType(1));
    return result;
}

template<class ValueType>
Polynomial<ValueType> SpeedTests::get_sigma(int variable_count, int sigma_index) {
    if (sigma_index == 0) {
        return Polynomial<ValueType>(1);
    }
    if (variable_count == sigma_index) {
        Monomial sigma;
        for (int i = 0; i < variable_count; ++i) {
            sigma.set_degree(i, 1);
        }
        return Polynomial<ValueType>(sigma);
    }
    Monomial x_n;
    x_n.set_degree(variable_count - 1, 1);
    return get_sigma<ValueType>(variable_count - 1, sigma_index) + get_sigma<ValueType>(variable_count - 1, sigma_index - 1) * x_n;
}

}  // grobner

#endif  // GROBNER_SPEED_TESTS_H_
