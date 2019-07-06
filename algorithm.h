#ifndef GROBNER_ALGORITHM_H_
#define GROBNER_ALGORITHM_H_

#include "monomial.h"
#include "monomial_order.h"
#include "polynomial.h"
#include "polynomial_set.h"

namespace grobner {

class Algorithm {
  public:
    template<class Order, class ValueType>
    static void extend_to_grobners_basis(PolynomialSet<ValueType>& F);

    template<class Order, class ValueType>
    static std::pair<Monomial, ValueType> get_leading_monomial(const Polynomial<ValueType>& f);

    // tries to reduce g by f, returns true if the reduction was made
    template<class Order, class ValueType>
    static bool reduce_by(Polynomial<ValueType>& g, const Polynomial<ValueType>& f);

    template<class Order, class ValueType>
    static void reduce_by(Polynomial<ValueType>& g, const PolynomialSet<ValueType>& F);

    template<class Order, class ValueType>
    static Polynomial<ValueType> get_S(const Polynomial<ValueType>& f, const Polynomial<ValueType>& g);

    static Monomial get_LCM(const Monomial& f, const Monomial& g);


  private:
    // tries to reduce g by F, returns true if the reduction was made
    template<class Order, class ValueType>
    static bool make_reduction_step(Polynomial<ValueType>& g, const PolynomialSet<ValueType>& F);

    // tries to make one step of Buchberger's algorithm, returns true if the step was made
    template<class Order, class ValueType>
    static bool make_buchberger_step(PolynomialSet<ValueType>& F);    
};


template<class Order, class ValueType>
void Algorithm::extend_to_grobners_basis(PolynomialSet<ValueType>& F) {
    while (make_buchberger_step<Order>(F));
}

template<class Order, class ValueType>
std::pair<Monomial, ValueType> Algorithm::get_leading_monomial(const Polynomial<ValueType>& f) {
    bool is_first = true;
    Monomial result;
    ValueType result_coefficient;

    for (auto [monomial, coefficient] : f) {
        if (is_first || Order()(result, monomial)) {
            result = monomial;
            result_coefficient = coefficient;
        }
        is_first = false;
    }
    return {result, result_coefficient};
}

template<class Order, class ValueType>
bool Algorithm::reduce_by(Polynomial<ValueType>& g, const Polynomial<ValueType>& f) {
    if (f.is_zero()) {
        return false;
    }
    auto [f_lead, f_coefficient] = get_leading_monomial<Order>(f);
    for (auto [monomial, coefficient] : g) {
        if (monomial.is_divisible_by(f_lead)) {
            g -= f * (monomial / f_lead) * (coefficient / f_coefficient);
            return true;
        }
    }
    return false;
}

template<class Order, class ValueType>
void Algorithm::reduce_by(Polynomial<ValueType>& g, const PolynomialSet<ValueType>& F) {
    while (make_reduction_step<Order>(g, F));
}

template<class Order, class ValueType>
Polynomial<ValueType> Algorithm::get_S(const Polynomial<ValueType>& f, const Polynomial<ValueType>& g) {
    auto [f_lead, f_coefficient] = get_leading_monomial<Order>(f);
    auto [g_lead, g_coefficient] = get_leading_monomial<Order>(g);
    auto lcm = get_LCM(f_lead, g_lead);
    return f * (lcm / f_lead) / f_coefficient - g * (lcm / g_lead) / g_coefficient;
}

Monomial Algorithm::get_LCM(const Monomial& f, const Monomial& g) {
    Monomial result;
    for (size_t i = 0; i < std::min(f.size(), g.size()); ++i) {
        result.set_degree(i, std::max(f[i], g[i]));
    }
    for (size_t i = f.size(); i < g.size(); ++i) {
        result.set_degree(i, g[i]);
    }
    for (size_t i = g.size(); i < f.size(); ++i) {
        result.set_degree(i, f[i]);
    }
    return result;
}


template<class Order, class ValueType>
bool Algorithm::make_reduction_step(Polynomial<ValueType>& g, const PolynomialSet<ValueType>& F) {
    for (auto f : F) {
        if (reduce_by<Order>(g, f)) {
            return true;
        }
    }
    return false;
}

template<class Order, class ValueType>
bool Algorithm::make_buchberger_step(PolynomialSet<ValueType>& F) {
    for (auto it = F.pbegin(); it != F.pend(); ++it) {
        auto S = get_S<Order>(*((*it).first), *((*it).second));
        reduce_by<Order>(S, F);
        if (!S.is_zero()) {
            F.insert(S);
            return true;
        }
    }
    return false;
}

}  // grobner

#endif  // GROBNER_ALGORITHM_H_
