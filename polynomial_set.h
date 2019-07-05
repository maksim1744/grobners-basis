#ifndef GROBNER_POLYNOMIAL_SET_H_
#define GROBNER_POLYNOMIAL_SET_H_

#include <unordered_set>

#include "declarations.h"
#include "hashes.h"
#include "polynomial.h"

namespace grobner {

template<class ElemIt>
class PairSetIterator;

template<class ValueType>
class PolynomialSet {
  public:
    using Polynomial = Polynomial<ValueType>;
    using Container = std::unordered_set<Polynomial, std::hash<Polynomial>>;
    using iterator = typename Container::iterator;
    using const_iterator = typename Container::const_iterator;
    using pair_iterator = PairSetIterator<iterator>;

    bool operator == (const PolynomialSet& other) const;
    bool operator != (const PolynomialSet& other) const;

    PolynomialSet();
    PolynomialSet(const std::initializer_list<Polynomial>& list);

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
    Container data;
};

template<class ElemIt>
class PairSetIterator {
  public:
    PairSetIterator(ElemIt first, ElemIt second, ElemIt c_begin, ElemIt c_end);

    PairSetIterator<ElemIt> operator++();

    std::pair<ElemIt, ElemIt> operator * ();

    bool operator != (const PairSetIterator& other);

  private:
    ElemIt first;
    ElemIt second;
    ElemIt c_begin;
    ElemIt c_end;
};

template<class ElemIt>
PairSetIterator<ElemIt>::PairSetIterator(ElemIt first, ElemIt second, ElemIt c_begin, ElemIt c_end) :
    first(first),
    second(second),
    c_begin(c_begin),
    c_end(c_end) {
}

template<class ElemIt>
PairSetIterator<ElemIt> PairSetIterator<ElemIt>::operator++() {
    ++second;
    if (first == second) {
        ++first;
        second = c_begin;
    }
    return *this;
}

template<class ElemIt>
std::pair<ElemIt, ElemIt> PairSetIterator<ElemIt>::operator * () {
    return {first, second};
}

template<class ElemIt>
bool PairSetIterator<ElemIt>::operator != (const PairSetIterator& other) {
    return first != other.first || second != other.second;
}


template<class ValueType>
PolynomialSet<ValueType>::PolynomialSet() {}

template<class ValueType>
PolynomialSet<ValueType>::PolynomialSet(const std::initializer_list<Polynomial>& list) : data(list) {}

template<class ValueType>
bool PolynomialSet<ValueType>::operator == (const PolynomialSet& other) const {
    return data == other.data;
}

template<class ValueType>
bool PolynomialSet<ValueType>::operator != (const PolynomialSet& other) const {
    return data != other.data;
}

template<class ValueType>
void PolynomialSet<ValueType>::insert(const Polynomial& polynomial) {
    data.insert(polynomial);
}

template<class ValueType>
void PolynomialSet<ValueType>::erase(const Polynomial& polynomial) {
    data.erase(polynomial);
}

template<class ValueType>
void PolynomialSet<ValueType>::clear() {
    data.clear();
}

template<class ValueType>
bool PolynomialSet<ValueType>::empty() const {
    return data.empty();
}

template<class ValueType>
size_t PolynomialSet<ValueType>::size() const {
    return data.size();
}

template<class ValueType>
typename PolynomialSet<ValueType>::iterator PolynomialSet<ValueType>::begin() {
    return data.begin();
}

template<class ValueType>
typename PolynomialSet<ValueType>::iterator PolynomialSet<ValueType>::end() {
    return data.end();
}

template<class ValueType>
typename PolynomialSet<ValueType>::const_iterator PolynomialSet<ValueType>::begin() const {
    return data.begin();
}

template<class ValueType>
typename PolynomialSet<ValueType>::const_iterator PolynomialSet<ValueType>::end() const {
    return data.end();
}

template<class ValueType>
typename PolynomialSet<ValueType>::pair_iterator PolynomialSet<ValueType>::pbegin() {
    return pair_iterator(++begin(), begin(), begin(), end());
}

template<class ValueType>
typename PolynomialSet<ValueType>::pair_iterator PolynomialSet<ValueType>::pend() {
    return pair_iterator(end(), begin(), begin(), end());
}

}  // grobner

#endif  // GROBNER_POLYNOMIAL_SET_H_
