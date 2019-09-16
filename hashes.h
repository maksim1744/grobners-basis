#ifndef GROBNER_HASHES_H_
#define GROBNER_HASHES_H_

#include <boost/rational.hpp>

#include "declarations.h"
#include "modular.h"
#include "monomial.h"
#include "polynomial.h"

namespace grobner {

template<class T>
struct hash {
    size_t operator()(const T& value) const;
};

template<class ValueType>
struct hash<grobner::Polynomial<ValueType>> {
    size_t operator()(const grobner::Polynomial<ValueType>& polynomial) const;
};

template<>
struct hash<grobner::Monomial> {
    size_t operator()(const grobner::Monomial& monomial) const;
};

template<auto P>
struct hash<grobner::Modular<P>> {
    size_t operator()(const grobner::Modular<P>& modular) const;
};

template<class ValueType>
struct hash<boost::rational<ValueType>> {
    size_t operator()(const boost::rational<ValueType>& rational) const;
};


template<class T>
size_t hash<T>::operator()(const T& value) const {
    return std::hash<T>()(value);
}

template<class ValueType>
size_t hash<grobner::Polynomial<ValueType>>::operator()(const grobner::Polynomial<ValueType>& polynomial) const {
    using Modular = grobner::Modular<1'000'000'007ll>;
    Modular result = 0;
    for (auto [monomial, coefficient] : polynomial) {
        result += Modular(hash<grobner::Monomial>()(monomial)).get_value() ^ hash<ValueType>()(coefficient);
    }
    return result.get_value();
}

size_t hash<grobner::Monomial>::operator()(const grobner::Monomial& monomial) const {
    using Modular = grobner::Modular<1'000'000'007ll>;
    Modular result = 0;
    Modular current = 1;
    Modular P = 942437;  // just random prime number
    for (int i = 0; i < monomial.container_size(); ++i) {
        result += current * Modular(monomial.get_degree(i));
        current *= P;
    }
    return result.get_value();
}

template<auto P>
size_t hash<grobner::Modular<P>>::operator()(const grobner::Modular<P>& modular) const {
    return hash<typename grobner::Modular<P>::ValueType>()(modular.get_value());
}

template<class ValueType>
size_t hash<boost::rational<ValueType>>::operator()(const boost::rational<ValueType>& rational) const {
    return hash<ValueType>()(rational.numerator()) ^ hash<ValueType>()(rational.denominator());
}

}  // std

#endif  // GROBNER_HASHES_H_
