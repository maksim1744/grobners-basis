#ifndef GROBNER_ORDER_H_
#define GROBNER_ORDER_H_

#include "monomial.h"

namespace grobner {

template<class FirstOrder, class SecondOrder>
class OrderSum {
  public:
    static int cmp(const Monomial& first, const Monomial& second);

    bool operator()(const Monomial& first, const Monomial& second) const;
};

class DegOrder {
  public:
    static int cmp(const Monomial& first, const Monomial& second);

    bool operator()(const Monomial& first, const Monomial& second) const;
};

class LexOrder {
  public:
    static int cmp(const Monomial& first, const Monomial& second);

    bool operator()(const Monomial& first, const Monomial& second) const;
};

class RevLexOrder {
  public:
    static int cmp(const Monomial& first, const Monomial& second);

    bool operator()(const Monomial& first, const Monomial& second) const;
};


template<class FirstOrder, class SecondOrder>
int OrderSum<FirstOrder, SecondOrder>::cmp(const Monomial& first, const Monomial& second) {
    int cmp_first = FirstOrder::cmp(first, second);
    if (cmp_first == 0) {
        return SecondOrder::cmp(first, second);
    }
    return cmp_first;
}

template<class FirstOrder, class SecondOrder>
bool OrderSum<FirstOrder, SecondOrder>::operator()(const Monomial& first, const Monomial& second) const {
    return cmp(first, second) < 0;
}


int DegOrder::cmp(const Monomial& first, const Monomial& second) {
    auto first_degree = first.get_degree();
    auto second_degree = second.get_degree();
    if (first_degree < second_degree) {
        return -1;
    } else if (first_degree == second_degree) {
        return 0;
    } else {
        return 1;
    }
}

bool DegOrder::operator()(const Monomial& first, const Monomial& second) const {
    return cmp(first, second) < 0;
}


int LexOrder::cmp(const Monomial& first, const Monomial& second) {
    for (size_t i = 0; i < std::max(first.container_size(), second.container_size()); ++i) {
        if (first.get_degree(i) < second.get_degree(i)) {
            return -1;
        } else if (first.get_degree(i) > second.get_degree(i)) {
            return 1;
        }
    }
    return 0;
}

bool LexOrder::operator()(const Monomial& first, const Monomial& second) const {
    return cmp(first, second) < 0;
}


int RevLexOrder::cmp(const Monomial& first, const Monomial& second) {
    for (size_t i = 0; i < std::max(first.container_size(), second.container_size()); ++i) {
        if (first.get_degree(i) < second.get_degree(i)) {
            return 1;
        } else if (first.get_degree(i) > second.get_degree(i)) {
            return -1;
        }
    }
    return 0;
}

bool RevLexOrder::operator()(const Monomial& first, const Monomial& second) const {
    return cmp(first, second) < 0;
}

}  // grobner

#endif  // GROBNER_ORDER_H_
