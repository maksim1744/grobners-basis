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
    static void extend_to_grobners_basis(PolynomialSet<ValueType>* F);

    template<class Order, class ValueType>
    static std::pair<Monomial, ValueType> get_leading_term(const Polynomial<ValueType>& f);

    template<class Order, class ValueType>
    static void reduce_by(const Polynomial<ValueType>& f, Polynomial<ValueType>* g);

    template<class Order, class ValueType>
    static void reduce_by(const PolynomialSet<ValueType>& F, Polynomial<ValueType>* g);

    template<class Order, class ValueType>
    static Polynomial<ValueType> get_S(const Polynomial<ValueType>& f, const Polynomial<ValueType>& g);

    static Monomial get_LCM(const Monomial& f, const Monomial& g);


  private:
    // tries to reduce g by f, returns true if the reduction was made
    template<class Order, class ValueType>
    static bool make_reduction_step(const Polynomial<ValueType>& f, Polynomial<ValueType>* g);

    // tries to make one step of Buchberger's algorithm, returns true if the step was made
    template<class Order, class ValueType>
    static bool make_buchberger_step(PolynomialSet<ValueType>* F);    
};


template<class Order, class ValueType>
void Algorithm::extend_to_grobners_basis(PolynomialSet<ValueType>* F) {
    while (make_buchberger_step<Order>(F));
}

template<class Order, class ValueType>
std::pair<Monomial, ValueType> Algorithm::get_leading_term(const Polynomial<ValueType>& f) {
    bool is_first = true;
    Monomial result;
    ValueType result_coefficient;

    for (const auto& [monomial, coefficient] : f) {
        if (is_first || Order()(result, monomial)) {
            result = monomial;
            result_coefficient = coefficient;
        }
        is_first = false;
    }
    return {result, result_coefficient};
}

template<class Order, class ValueType>
void Algorithm::reduce_by(const Polynomial<ValueType>& f, Polynomial<ValueType>* g) {
    while (make_reduction_step<Order>(f, g));
}

template<class Order, class ValueType>
void Algorithm::reduce_by(const PolynomialSet<ValueType>& F, Polynomial<ValueType>* g) {
    for (const auto& f : F) {
        reduce_by<Order>(f, g);
    }
}

template<class Order, class ValueType>
Polynomial<ValueType> Algorithm::get_S(const Polynomial<ValueType>& f, const Polynomial<ValueType>& g) {
    auto [f_lead, f_coefficient] = get_leading_term<Order>(f);
    auto [g_lead, g_coefficient] = get_leading_term<Order>(g);
    auto lcm = get_LCM(f_lead, g_lead);
    return f * (lcm / f_lead) / f_coefficient - g * (lcm / g_lead) / g_coefficient;
}

Monomial Algorithm::get_LCM(const Monomial& f, const Monomial& g) {
    Monomial result;
    for (size_t i = 0; i < std::max(f.container_size(), g.container_size()); ++i) {
        result.set_degree(i, std::max(f.get_degree(i), g.get_degree(i)));
    }
    return result;
}


template<class Order, class ValueType>
bool Algorithm::make_reduction_step(const Polynomial<ValueType>& f, Polynomial<ValueType>* g) {
    if (f.is_zero()) {
        return false;
    }
    auto [f_lead, f_coefficient] = get_leading_term<Order>(f);
    for (const auto& [monomial, coefficient] : *g) {
        if (monomial.is_divisible_by(f_lead)) {
            *g -= f * (monomial / f_lead) * (coefficient / f_coefficient);
            return true;
        }
    }
    return false;
}

template<class Order, class ValueType>
bool Algorithm::make_buchberger_step(PolynomialSet<ValueType>* F) {
    for (auto it = F->pbegin(); it != F->pend(); ++it) {
        auto S = get_S<Order>(it.first(), it.second());
        reduce_by<Order>(*F, &S);
        if (!S.is_zero()) {
            F->insert(S);
            return true;
        }
    }
    return false;
}

}  // grobner

#endif  // GROBNER_ALGORITHM_H_
