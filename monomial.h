#ifndef GROBNER_MONOMIAL_H_
#define GROBNER_MONOMIAL_H_

#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#include "modular.h"

namespace grobner {

class Monomial {
  public:
    using DegreeType = int;

    Monomial();
    Monomial(const std::initializer_list<DegreeType>& degrees);
    // use a..z as variables, for example: c^3bd^-3c^-1
    Monomial(const std::string& s);

    DegreeType get_degree() const;

    bool is_divisible_by(const Monomial& other) const;

    void read(std::stringstream& ss);

    void clear();
    size_t size() const;
    const DegreeType& operator[] (size_t i) const;
    void set_degree(size_t i, DegreeType degree);

    Monomial& operator *= (const Monomial& other);
    Monomial operator * (const Monomial& other) const;

    Monomial& operator /= (const Monomial& other);
    Monomial operator / (const Monomial& other) const;

    bool operator == (const Monomial& other) const;
    bool operator != (const Monomial& other) const;

    friend std::ostream& operator << (std::ostream& out, const Monomial& monomial);

  private:
    std::vector<DegreeType> degrees;

    bool read_variable(std::stringstream& ss);
    void remove_last_zeros();
};

Monomial::Monomial() {}

Monomial::Monomial(const std::initializer_list<DegreeType>& degrees) : degrees(degrees) {
    remove_last_zeros();
}

Monomial::Monomial(const std::string& s) {
    std::stringstream ss(s);
    read(ss);
    remove_last_zeros();
}


typename Monomial::DegreeType Monomial::get_degree() const {
    return std::accumulate(degrees.begin(), degrees.end(), DegreeType(0));
}


bool Monomial::is_divisible_by(const Monomial& other) const {
    for (size_t i = 0; i < std::min(size(), other.size()); ++i) {
        if ((*this)[i] < other[i]) {
            return false;
        }
    }
    for (size_t i = size(); i < other.size(); ++i) {
        if (other[i] != DegreeType(0)) {
            return false;
        }
    }
    return true;
}


void Monomial::read(std::stringstream& ss) {
    clear();
    while (read_variable(ss));
    remove_last_zeros();
}

void Monomial::clear()  {
    degrees.clear();
}

size_t Monomial::size() const {
    return degrees.size();
}

const typename Monomial::DegreeType& Monomial::operator[] (size_t i) const {
    return degrees[i];
}

void Monomial::set_degree(size_t i, DegreeType degree) {
    if (size() <= i) {
        degrees.resize(i + 1, 0);
    }
    degrees[i] = degree;
    remove_last_zeros();
}


Monomial& Monomial::operator *= (const Monomial& other) {
    if (size() < other.size()) {
        degrees.resize(other.size(), 0);
    }
    for (size_t i = 0; i < other.size(); ++i) {
        degrees[i] += other[i];
    }
    remove_last_zeros();
    return *this;
}

Monomial Monomial::operator * (const Monomial& other) const {
    Monomial result = *this;
    result *= other;
    return result;
}


Monomial& Monomial::operator /= (const Monomial& other) {
    if (size() < other.size()) {
        degrees.resize(other.size(), 0);
    }
    for (size_t i = 0; i < other.size(); ++i) {
        degrees[i] -= other[i];
    }
    remove_last_zeros();
    return *this;
}

Monomial Monomial::operator / (const Monomial& other) const {
    Monomial result = *this;
    result /= other;
    return result;
}

bool Monomial::operator == (const Monomial& other) const {
    return (degrees == other.degrees);
}

bool Monomial::operator != (const Monomial& other) const {
    return (degrees != other.degrees);
}

std::ostream& operator << (std::ostream& out, const Monomial& monomial) {
    if (monomial.size() <= 26) {  // then we can use a..z as variables, otherwise x1,x2,...
        for (size_t i = 0; i < monomial.size(); ++i) {
            if (monomial[i] != 0) {
                out << (char)('a' + i);
                if (monomial[i] != 1) {
                    out << '^' << monomial[i];
                }
            }
        }
    } else {
        for (size_t i = 0; i < monomial.size(); ++i) {
            if (monomial[i] != 0) {
                out << 'x' << (i + 1);
                if (monomial[i] != 1) {
                    out << '^' << monomial[i];
                }
            }
        }
    }
    return out;
}

bool Monomial::read_variable(std::stringstream& ss) {
    if (ss.eof() || !std::islower(ss.peek())) {
        return false;
    }
    char variable;
    ss >> variable;
    typename Monomial::DegreeType degree = 1;
    if (!ss.eof() && ss.peek() == '^') {
        ss.get();
        ss >> degree;
    }
    if (degrees.size() <= variable - 'a') {
        degrees.resize(variable - 'a' + 1, 0);
    }
    degrees[variable - 'a'] += degree;
    return true;
}

void Monomial::remove_last_zeros() {
    while (!degrees.empty() && degrees.back() == DegreeType(0)) {
        degrees.pop_back();
    }
}

}  // grobner

#endif  // GROBNER_MONOMIAL_H_
