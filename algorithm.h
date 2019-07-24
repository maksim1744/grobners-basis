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

    // returns true if something changed (g ~> g_0 != g)
    template<class Order, class ValueType>
    static bool reduce_by(const Polynomial<ValueType>& f, Polynomial<ValueType>* g);

    template<class Order, class ValueType>
    static void reduce_by(const PolynomialSet<ValueType>& F, Polynomial<ValueType>* g);

    template<class Order, class ValueType>
    static Polynomial<ValueType> get_S(const Polynomial<ValueType>& f, const Polynomial<ValueType>& g);

    static Monomial get_LCM(const Monomial& f, const Monomial& g);


  private:
    // tries to reduce g by f, returns true if the reduction was made
    template<class Order, class ValueType>
    static bool make_reduction_step(const Polynomial<ValueType>& f, const Monomial& f_lead, const ValueType& f_coefficient, Polynomial<ValueType>* g);

    // tries to reduce g by F, returns true if the reduction was made
    template<class Order, class ValueType>
    static bool make_reduction_step(const PolynomialSet<ValueType>& F, Polynomial<ValueType>* g);

    template<class Order, class ValueType>
    static void add_new_s(const PolynomialSet<ValueType>& F, const Polynomial<ValueType>& new_f, PolynomialSet<ValueType>* set_of_s);
};


template<class Order, class ValueType>
void Algorithm::extend_to_grobners_basis(PolynomialSet<ValueType>* F) {
    PolynomialSet<ValueType> set_of_s;
    for (auto it = F->pbegin(); it != F->pend(); ++it) {
        auto S = get_S<Order>(it.first(), it.second());
        reduce_by<Order>(*F, &S);
        if (!S.is_zero()) {
            set_of_s.insert(S);
        }
    }
    while (!set_of_s.empty()) {
        auto S = set_of_s.retrieve_first();
        reduce_by<Order>(*F, &S);
        if (!S.is_zero()) {
            add_new_s<Order>(*F, S, &set_of_s);
            F->insert(S);
        }
    }
}

template<class Order, class ValueType>
std::pair<Monomial, ValueType> Algorithm::get_leading_term(const Polynomial<ValueType>& f) {
    assert(!f.is_zero());

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
bool Algorithm::reduce_by(const Polynomial<ValueType>& f, Polynomial<ValueType>* g) {
    assert(!f.is_zero());
    auto [f_lead, f_coefficient] = get_leading_term<Order>(f);

    bool something_changed = false;
    while (make_reduction_step<Order>(f, f_lead, f_coefficient, g)) {
        something_changed = true;
    }
    return something_changed;
}

template<class Order, class ValueType>
void Algorithm::reduce_by(const PolynomialSet<ValueType>& F, Polynomial<ValueType>* g) {
    while (make_reduction_step<Order>(F, g));
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
bool Algorithm::make_reduction_step(const Polynomial<ValueType>& f, const Monomial& f_lead, const ValueType& f_coefficient, Polynomial<ValueType>* g) {
    for (const auto& [monomial, coefficient] : *g) {
        if (monomial.is_divisible_by(f_lead)) {
            *g -= f * (monomial / f_lead) * (coefficient / f_coefficient);
            return true;
        }
    }
    return false;
}

template<class Order, class ValueType>
bool Algorithm::make_reduction_step(const PolynomialSet<ValueType>& F, Polynomial<ValueType>* g) {
    bool something_changed = false;
    for (const auto& f : F) {
        if (reduce_by<Order>(f, g)) {
            something_changed = true;
        }
    }
    return something_changed;
}

template<class Order, class ValueType>
void Algorithm::add_new_s(const PolynomialSet<ValueType>& F, const Polynomial<ValueType>& new_f, PolynomialSet<ValueType>* set_of_s) {
    for (const auto& g : F) {
        auto S = get_S<Order>(new_f, g);
        reduce_by<Order>(F, &S);
        if (!S.is_zero()) {
            set_of_s->insert(S);
        }
    }
}

}  // grobner

#endif  // GROBNER_ALGORITHM_H_
