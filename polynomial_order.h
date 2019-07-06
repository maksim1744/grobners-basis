#ifndef GROBNER_POLYNOMIAL_ORDER_H_
#define GROBNER_POLYNOMIAL_ORDER_H_

#include "polynomial.h"

#include <algorithm>

namespace grobner {

template<class MonomialOrder>
class PolynomialOrder {
  public:
    template<class ValueType>
    static int cmp(const Polynomial<ValueType>& first, const Polynomial<ValueType>& second);

    template<class ValueType>
    bool operator()(const Polynomial<ValueType>& first, const Polynomial<ValueType>& second) const;
};

template<class MonomialOrder>
template<class ValueType>
int PolynomialOrder<MonomialOrder>::cmp(const Polynomial<ValueType>& first, const Polynomial<ValueType>& second) {
    MonomialOrder monomial_order;
    auto monomials_first = first.get_monomials_sorted(monomial_order);
    auto monomials_second = second.get_monomials_sorted(monomial_order);
    std::reverse(monomials_first.begin(), monomials_first.end());
    std::reverse(monomials_second.begin(), monomials_second.end());

    for (size_t i = 0; i < std::min(monomials_first.size(), monomials_second.size()); ++i) {
        int cmp = MonomialOrder::cmp(monomials_first[i], monomials_second[i]);
        if (cmp != 0) {
            return cmp;
        }
    }
    if (monomials_first.size() < monomials_second.size()) {
        return -1;
    } else if (monomials_first.size() > monomials_second.size()) {
        return 1;
    } else {
        return 0;
    }
}

template<class MonomialOrder>
template<class ValueType>
bool PolynomialOrder<MonomialOrder>::operator()(const Polynomial<ValueType>& first, const Polynomial<ValueType>& second) const {
    return cmp(first, second) < 0;
}

}  // grobner

#endif  // GROBNER_POLYNOMIAL_ORDER_H_
