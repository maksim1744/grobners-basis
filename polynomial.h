#ifndef GROBNER_POLYNOMIAL_H_
#define GROBNER_POLYNOMIAL_H_

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

#include "hashes.h"
#include "monomial.h"

namespace grobner {

template<class ValueType>  // field, number of variables
class Polynomial {
  public:
    using Containter = std::unordered_map<Monomial, ValueType, std::hash<Monomial>>;
    using iterator = typename Containter::iterator;
    using const_iterator = typename Containter::const_iterator;

    Polynomial();
    Polynomial(const Monomial& monomial);
    Polynomial(const ValueType& value);
    Polynomial(const std::string& s);
    Polynomial(const char* c);

    void add_monomial(const Monomial& monomial, ValueType coefficient = ValueType(1));

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    bool is_zero() const;
    void set_to_zero();

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

    Containter data;
};

template<class ValueType>
Polynomial<ValueType>::Polynomial() {}

template<class ValueType>
Polynomial<ValueType>::Polynomial(const Monomial& monomial) {
    data[monomial] = ValueType(1);
}

template<class ValueType>
Polynomial<ValueType>::Polynomial(const ValueType& value) {
    if (value != ValueType(0)) {
        data[Monomial()] = value;
    }
}

template<class ValueType>
Polynomial<ValueType>::Polynomial(const std::string& s) {
    std::stringstream ss(s);
    while (!ss.eof()) {
        ValueType coefficient = read_coefficient(ss);
        Monomial monomial;
        monomial.read(ss);
        data[monomial] += coefficient;
    }
    remove_zero_coefficients();
}

template<class ValueType>
Polynomial<ValueType>::Polynomial(const char* c) : Polynomial(std::string(c)) {}

template<class ValueType>
void Polynomial<ValueType>::add_monomial(const Monomial& monomial, ValueType coefficient) {
    data[monomial] += coefficient;
    if (data[monomial] == ValueType(0)) {
        data.erase(monomial);
    }
}


template<class ValueType>
typename Polynomial<ValueType>::iterator Polynomial<ValueType>::begin() {
    return data.begin();
}

template<class ValueType>
typename Polynomial<ValueType>::iterator Polynomial<ValueType>::end() {
    return data.end();
}

template<class ValueType>
typename Polynomial<ValueType>::const_iterator Polynomial<ValueType>::begin() const {
    return data.begin();
}

template<class ValueType>
typename Polynomial<ValueType>::const_iterator Polynomial<ValueType>::end() const {
    return data.end();
}

template<class ValueType>
bool Polynomial<ValueType>::is_zero() const {
    return data.empty();
}

template<class ValueType>
void Polynomial<ValueType>::set_to_zero() {
    data.clear();
}


template<class ValueType>
Polynomial<ValueType>& Polynomial<ValueType>::operator += (const Polynomial& other) {
    for (auto [monomial, coefficient] : other) {
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
    for (auto [monomial, coefficient] : other) {
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
    for (auto [monomial, coefficient] : *this) {
        result.add_monomial(monomial * other, coefficient);
    }
    return result;
}

template<class ValueType>
Polynomial<ValueType>& Polynomial<ValueType>::operator *= (const ValueType& other) {
    if (other == ValueType(0)) {
        set_to_zero();
    } else {
        for (auto& [monomial, coefficient] : *this) {
            coefficient *= other;
        }
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
    for (auto [monomial, coefficient] : other) {
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
    for (auto [monomial, coefficient] : *this) {
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
    return data == other.data;
}

template<class ValueType>
bool Polynomial<ValueType>::operator != (const Polynomial& other) const {
    return data != other.data;
}




template<class ValueType>
std::ostream& operator << (std::ostream& out, const Polynomial<ValueType>& polynomial) {
    if (polynomial.empty()) {
        return out << ValueType(0);
    }
    bool is_first = true;
    for (auto [monomial, coefficient] : polynomial) {
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
    auto it = data.begin();
    while (it != data.end()) {
        auto it_next = it;
        ++it_next;
        if (it->second == ValueType(0)) {
            data.erase(it);
        }
        it = it_next;
    }
}

}  // grobner

#endif  // GROBNER_POLYNOMIAL_H_
