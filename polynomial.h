#ifndef GROBNER_POLYNOMIAL_H_
#define GROBNER_POLYNOMIAL_H_

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "hashes.h"
#include "monomial.h"

namespace grobner {

template<class ValueType>
class Polynomial {
  public:
    using Containter = std::unordered_map<Monomial, ValueType, grobner::hash<Monomial>>;
    using iterator = typename Containter::iterator;
    using const_iterator = typename Containter::const_iterator;

    Polynomial();
    Polynomial(Monomial monomial);
    Polynomial(ValueType value);
    Polynomial(std::string s);
    Polynomial(const char* c);

    void add_monomial(const Monomial& monomial, ValueType coefficient = ValueType(1));

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    bool is_zero() const;
    void set_to_zero();

    template<class MonomialOrder>
    std::vector<Monomial> get_monomials_sorted(const MonomialOrder& monomial_order) const;

    Polynomial& operator += (const Polynomial& other);
    Polynomial operator + (const Polynomial& other) const;
    Polynomial& operator -= (const Polynomial& other);
    Polynomial operator - (const Polynomial& other) const;

    Polynomial& operator *= (const Monomial& other);
    Polynomial operator * (const Monomial& other) const;
    Polynomial& operator *= (const ValueType& other);
    Polynomial operator * (const ValueType& other) const;
    Polynomial& operator *= (const Polynomial& other);
    Polynomial operator * (const Polynomial& other) const;

    Polynomial& operator /= (const Monomial& other);
    Polynomial operator / (const Monomial& other) const;
    Polynomial& operator /= (const ValueType& other);
    Polynomial operator / (const ValueType& other) const;

    bool operator == (const Polynomial& other) const;
    bool operator != (const Polynomial& other) const;

    template<class OtherValueType>
    friend std::ostream& operator << (std::ostream& out, const Polynomial<OtherValueType>& polynomial);

  private:
    std::string to_string(const ValueType& value) const;
    ValueType read_coefficient(std::stringstream& ss) const;
    void remove_zero_coefficients();

    Containter data_;
};

template<class ValueType>
Polynomial<ValueType>::Polynomial() {}

template<class ValueType>
Polynomial<ValueType>::Polynomial(Monomial monomial) {
    data_.emplace(monomial, 1);
}

template<class ValueType>
Polynomial<ValueType>::Polynomial(ValueType value) {
    if (value != ValueType(0)) {
        data_[Monomial()] = value;
    }
}

template<class ValueType>
Polynomial<ValueType>::Polynomial(std::string s) {
    std::stringstream ss(s);
    while (!ss.eof()) {
        ValueType coefficient = read_coefficient(ss);
        Monomial monomial;
        monomial.read(ss);
        data_[monomial] += coefficient;
    }
    remove_zero_coefficients();
}

template<class ValueType>
Polynomial<ValueType>::Polynomial(const char* c) : Polynomial(std::string(c)) {}

template<class ValueType>
void Polynomial<ValueType>::add_monomial(const Monomial& monomial, ValueType coefficient) {
    data_[monomial] += coefficient;
    if (data_[monomial] == ValueType(0)) {
        data_.erase(monomial);
    }
}


template<class ValueType>
typename Polynomial<ValueType>::iterator Polynomial<ValueType>::begin() {
    return data_.begin();
}

template<class ValueType>
typename Polynomial<ValueType>::iterator Polynomial<ValueType>::end() {
    return data_.end();
}

template<class ValueType>
typename Polynomial<ValueType>::const_iterator Polynomial<ValueType>::begin() const {
    return data_.begin();
}

template<class ValueType>
typename Polynomial<ValueType>::const_iterator Polynomial<ValueType>::end() const {
    return data_.end();
}

template<class ValueType>
bool Polynomial<ValueType>::is_zero() const {
    return data_.empty();
}

template<class ValueType>
void Polynomial<ValueType>::set_to_zero() {
    data_.clear();
}


template<class ValueType>
template<class MonomialOrder>
std::vector<Monomial> Polynomial<ValueType>::get_monomials_sorted(const MonomialOrder& monomial_order) const {
    std::vector<Monomial> monomials;
    monomials.reserve(data_.size());
    for (const auto& [monomial, coefficient] : data_) {
        monomials.push_back(monomial);
    }
    std::sort(monomials.begin(), monomials.end(), monomial_order);
    return monomials;
}


template<class ValueType>
Polynomial<ValueType>& Polynomial<ValueType>::operator += (const Polynomial& other) {
    for (const auto& [monomial, coefficient] : other) {
        add_monomial(monomial, coefficient);
    }
}

template<class ValueType>
Polynomial<ValueType> Polynomial<ValueType>::operator + (const Polynomial& other) const {
    Polynomial result = *this;
    result += other;
    return result;
}

template<class ValueType>
Polynomial<ValueType>& Polynomial<ValueType>::operator -= (const Polynomial& other) {
    for (const auto& [monomial, coefficient] : other) {
        add_monomial(monomial, -coefficient);
    }
}

template<class ValueType>
Polynomial<ValueType> Polynomial<ValueType>::operator - (const Polynomial& other) const {
    Polynomial result = *this;
    result -= other;
    return result;
}

template<class ValueType>
Polynomial<ValueType>& Polynomial<ValueType>::operator *= (const Monomial& other) {
    *this = *this * other;
    return *this;
}

template<class ValueType>
Polynomial<ValueType> Polynomial<ValueType>::operator * (const Monomial& other) const {
    Polynomial result;
    for (const auto& [monomial, coefficient] : *this) {
        result.add_monomial(monomial * other, coefficient);
    }
    return result;
}

template<class ValueType>
Polynomial<ValueType>& Polynomial<ValueType>::operator *= (const ValueType& other) {
    if (other == ValueType(0)) {
        set_to_zero();
        return *this;
    }
    for (auto& [monomial, coefficient] : *this) {
        coefficient *= other;
    }
    return *this;
}

template<class ValueType>
Polynomial<ValueType> Polynomial<ValueType>::operator * (const ValueType& other) const {
    Polynomial result = *this;
    result *= other;
    return result;
}

template<class ValueType>
Polynomial<ValueType>& Polynomial<ValueType>::operator *= (const Polynomial& other) {
    *this = *this * other;
    return *this;
}

template<class ValueType>
Polynomial<ValueType> Polynomial<ValueType>::operator * (const Polynomial& other) const {
    Polynomial result;
    for (const auto& [monomial, coefficient] : other) {
        result += *this * monomial * coefficient;
    }
    return result;
}

template<class ValueType>
Polynomial<ValueType>& Polynomial<ValueType>::operator /= (const Monomial& other) {
    *this = *this / other;
    return *this;
}

template<class ValueType>
Polynomial<ValueType> Polynomial<ValueType>::operator / (const Monomial& other) const {
    Polynomial result;
    for (const auto& [monomial, coefficient] : *this) {
        result.add_monomial(monomial / other, coefficient);
    }
    return result;
}

template<class ValueType>
Polynomial<ValueType>& Polynomial<ValueType>::operator /= (const ValueType& other) {
    for (auto& [monomial, coefficient] : *this) {
        coefficient /= other;
    }
    return *this;
}

template<class ValueType>
Polynomial<ValueType> Polynomial<ValueType>::operator / (const ValueType& other) const {
    Polynomial result = *this;
    result /= other;
    return result;
}

template<class ValueType>
bool Polynomial<ValueType>::operator == (const Polynomial& other) const {
    return data_ == other.data_;
}

template<class ValueType>
bool Polynomial<ValueType>::operator != (const Polynomial& other) const {
    return !(*this == other);
}




template<class ValueType>
std::ostream& operator << (std::ostream& out, const Polynomial<ValueType>& polynomial) {
    if (polynomial.empty()) {
        return out << ValueType(0);
    }
    bool is_first = true;
    for (const auto& [monomial, coefficient] : polynomial) {
        std::string coefficient_string = polynomial.to_string(coefficient);
        if (!coefficient_string.empty() && coefficient_string[0] != '-' && coefficient_string[0] != '+' && !is_first) {
            out << '+';
        }
        out << coefficient_string << monomial;
        is_first = false;
    }
    return out;
}



template<class ValueType>
std::string Polynomial<ValueType>::to_string(const ValueType& value) const {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

template<class ValueType>
ValueType Polynomial<ValueType>::read_coefficient(std::stringstream& ss) const {
    ValueType coefficient;
    ss >> coefficient;
    return coefficient;
}

template<class ValueType>
void Polynomial<ValueType>::remove_zero_coefficients() {
    auto it = data_.begin();
    while (it != data_.end()) {
        auto it_next = it;
        ++it_next;
        if (it->second == ValueType(0)) {
            data_.erase(it);
        }
        it = it_next;
    }
}

}  // grobner

#endif  // GROBNER_POLYNOMIAL_H_
