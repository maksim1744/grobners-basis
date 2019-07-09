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
    Monomial(std::initializer_list<DegreeType> degrees_);
    // use a..z as variables, for example: c^3bd^-3c^-1
    Monomial(std::string s);

    DegreeType get_degree() const;

    bool is_divisible_by(const Monomial& other) const;

    void read(std::stringstream& ss);

    void set_all_degrees_to_zero();
    size_t container_size() const;
    DegreeType get_degree(size_t i) const;
    void set_degree(size_t i, DegreeType degree);

    Monomial& operator *= (const Monomial& other);
    Monomial operator * (const Monomial& other) const;

    Monomial& operator /= (const Monomial& other);
    Monomial operator / (const Monomial& other) const;

    bool operator == (const Monomial& other) const;
    bool operator != (const Monomial& other) const;

    friend std::ostream& operator << (std::ostream& out, const Monomial& monomial);

  private:
    std::vector<DegreeType> degrees_;

    bool read_variable(std::stringstream& ss);
    void remove_last_zeros();
    void print_variable(std::ostream& out, size_t i, bool use_alphabet) const;
};

Monomial::Monomial() {}

Monomial::Monomial(std::initializer_list<DegreeType> degrees) : degrees_(degrees) {
    remove_last_zeros();
}

Monomial::Monomial(std::string s) {
    std::stringstream ss(s);
    read(ss);
    remove_last_zeros();
}


typename Monomial::DegreeType Monomial::get_degree() const {
    return std::accumulate(degrees_.begin(), degrees_.end(), DegreeType(0));
}


bool Monomial::is_divisible_by(const Monomial& other) const {
    for (size_t i = 0; i < std::min(container_size(), other.container_size()); ++i) {
        if (this->get_degree(i) < other.get_degree(i)) {
            return false;
        }
    }
    for (size_t i = container_size(); i < other.container_size(); ++i) {
        if (DegreeType(0) < other.get_degree(i)) {
            return false;
        }
    }
    return true;
}


void Monomial::read(std::stringstream& ss) {
    set_all_degrees_to_zero();
    while (read_variable(ss));
    remove_last_zeros();
}

void Monomial::set_all_degrees_to_zero()  {
    degrees_.clear();
}

size_t Monomial::container_size() const {
    return degrees_.size();
}

typename Monomial::DegreeType Monomial::get_degree(size_t i) const {
    if (i < degrees_.size()) {
        return degrees_[i];
    } else {
        return 0;
    }
}

void Monomial::set_degree(size_t i, DegreeType degree) {
    if (container_size() <= i) {
        degrees_.resize(i + 1, 0);
    }
    degrees_[i] = degree;
    remove_last_zeros();
}


Monomial& Monomial::operator *= (const Monomial& other) {
    if (container_size() < other.container_size()) {
        degrees_.resize(other.container_size(), 0);
    }
    for (size_t i = 0; i < other.container_size(); ++i) {
        degrees_[i] += other.get_degree(i);
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
    if (container_size() < other.container_size()) {
        degrees_.resize(other.container_size(), 0);
    }
    for (size_t i = 0; i < other.container_size(); ++i) {
        degrees_[i] -= other.get_degree(i);
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
    return (degrees_ == other.degrees_);
}

bool Monomial::operator != (const Monomial& other) const {
    return (degrees_ != other.degrees_);
}

std::ostream& operator << (std::ostream& out, const Monomial& monomial) {
    bool use_alphabet = (monomial.container_size() <= 26);  // then we can use a..z as variables, otherwise x1,x2,...
    for (size_t i = 0; i < monomial.container_size(); ++i) {
        monomial.print_variable(out, i, use_alphabet);
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
    if (degrees_.size() <= variable - 'a') {
        degrees_.resize(variable - 'a' + 1, 0);
    }
    degrees_[variable - 'a'] += degree;
    return true;
}

void Monomial::remove_last_zeros() {
    while (!degrees_.empty() && degrees_.back() == DegreeType(0)) {
        degrees_.pop_back();
    }
}

void Monomial::print_variable(std::ostream& out, size_t i, bool use_alphabet) const {
    if (get_degree(i) == 0) {
        return;
    }
    if (use_alphabet) {
        out << (char)('a' + i);
    } else {
        out << 'x' << (i + 1);
    }
    if (get_degree(i) != DegreeType(1)) {
        out << '^' << get_degree(i);
    }
}

}  // grobner

#endif  // GROBNER_MONOMIAL_H_
