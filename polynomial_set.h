#ifndef GROBNER_POLYNOMIAL_SET_H_
#define GROBNER_POLYNOMIAL_SET_H_

#include <unordered_set>

#include "declarations.h"
#include "hashes.h"
#include "pair_set_iterator.h"
#include "polynomial.h"

namespace grobner {

template<class ValueType>
class PolynomialSet {
  public:
    using Polynomial = Polynomial<ValueType>;
    using Container = std::unordered_set<Polynomial, grobner::hash<Polynomial>>;
    using iterator = typename Container::iterator;
    using const_iterator = typename Container::const_iterator;
    using pair_iterator = PairSetIterator<iterator>;

    bool operator == (const PolynomialSet& other) const;
    bool operator != (const PolynomialSet& other) const;

    PolynomialSet();
    PolynomialSet(std::initializer_list<Polynomial> list);

    void insert(const Polynomial& polynomial);
    void erase(const Polynomial& polynomial);
    void clear();
    bool empty() const;
    size_t size() const;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    pair_iterator pbegin();
    pair_iterator pend();

  private:
    Container data_;
};


template<class ValueType>
PolynomialSet<ValueType>::PolynomialSet() {}

template<class ValueType>
PolynomialSet<ValueType>::PolynomialSet(std::initializer_list<Polynomial> list) : data_(list) {}

template<class ValueType>
bool PolynomialSet<ValueType>::operator == (const PolynomialSet& other) const {
    return data_ == other.data_;
}

template<class ValueType>
bool PolynomialSet<ValueType>::operator != (const PolynomialSet& other) const {
    return data_ != other.data_;
}

template<class ValueType>
void PolynomialSet<ValueType>::insert(const Polynomial& polynomial) {
    data_.insert(polynomial);
}

template<class ValueType>
void PolynomialSet<ValueType>::erase(const Polynomial& polynomial) {
    data_.erase(polynomial);
}

template<class ValueType>
void PolynomialSet<ValueType>::clear() {
    data_.clear();
}

template<class ValueType>
bool PolynomialSet<ValueType>::empty() const {
    return data_.empty();
}

template<class ValueType>
size_t PolynomialSet<ValueType>::size() const {
    return data_.size();
}

template<class ValueType>
typename PolynomialSet<ValueType>::iterator PolynomialSet<ValueType>::begin() {
    return data_.begin();
}

template<class ValueType>
typename PolynomialSet<ValueType>::iterator PolynomialSet<ValueType>::end() {
    return data_.end();
}

template<class ValueType>
typename PolynomialSet<ValueType>::const_iterator PolynomialSet<ValueType>::begin() const {
    return data_.begin();
}

template<class ValueType>
typename PolynomialSet<ValueType>::const_iterator PolynomialSet<ValueType>::end() const {
    return data_.end();
}

template<class ValueType>
typename PolynomialSet<ValueType>::pair_iterator PolynomialSet<ValueType>::pbegin() {
    if (empty()) {
        return pend();
    }
    return pair_iterator(++begin(), begin(), begin(), end());
}

template<class ValueType>
typename PolynomialSet<ValueType>::pair_iterator PolynomialSet<ValueType>::pend() {
    return pair_iterator(end(), begin(), begin(), end());
}

}  // grobner

#endif  // GROBNER_POLYNOMIAL_SET_H_
