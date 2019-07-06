#ifndef GROBNER_MODULAR_H_
#define GROBNER_MODULAR_H_

#include <iostream>

namespace grobner {

// some methods work correctly only with prime P (get_inverse, division)
template<auto P>
class Modular {
  public:
    using ValueType = decltype(P);

    Modular(ValueType _value = 0);

    Modular& operator += (const Modular& other);
    Modular operator + (const Modular& other) const;
    Modular& operator -= (const Modular& other);
    Modular operator - (const Modular& other) const;
    Modular& operator *= (const Modular& other);
    Modular operator * (const Modular& other) const;
    Modular& operator /= (const Modular& other);
    Modular operator / (const Modular& other) const;
    Modular operator - () const;

    bool operator == (const Modular& other) const;
    bool operator != (const Modular& other) const;

    Modular get_pow(ValueType power) const;
    Modular get_inverse() const;
    ValueType get_value() const;

    template<auto Q>
    friend std::ostream& operator << (std::ostream& out, const Modular<Q>& other);
    template<auto Q>
    friend std::istream& operator >> (std::istream& in, Modular<Q>& other);

  private:
    ValueType value;
    void normalize();
};

template<auto P>
Modular<P>::Modular(typename Modular<P>::ValueType _value) : value(_value) {
    normalize();
}

template<auto P>
Modular<P>& Modular<P>::operator += (const Modular& other) {
    value += other.value;
    if (value >= P) {
        value -= P;
    }
    return *this;
}

template<auto P>
Modular<P> Modular<P>::operator + (const Modular& other) const {
    Modular<P> result = *this;
    return result += other;
}

template<auto P>
Modular<P>& Modular<P>::operator -= (const Modular& other) {
    value -= other.value;
    if (value < 0) {
        value += P;
    }
    return *this;
}

template<auto P>
Modular<P> Modular<P>::operator - (const Modular& other) const {
    Modular<P> result = *this;
    return result -= other;
}

template<auto P>
Modular<P>& Modular<P>::operator *= (const Modular& other) {
    value = value * other.value % P;
    return *this;
}

template<auto P>
Modular<P> Modular<P>::operator * (const Modular& other) const {
    Modular<P> result = *this;
    return result *= other;
}

template<auto P>
Modular<P>& Modular<P>::operator /= (const Modular& other) {
    (*this) *= other.get_inverse();
    return *this;
}

template<auto P>
Modular<P> Modular<P>::operator / (const Modular& other) const {
    Modular<P> result = *this;
    return result /= other;
}

template<auto P>
Modular<P> Modular<P>::operator - () const {
    return Modular(0) - (*this);
}

template<auto P>
bool Modular<P>::operator == (const Modular& other) const {
    return value == other.value;
}

template<auto P>
bool Modular<P>::operator != (const Modular& other) const {
    return value != other.value;
}


template<auto P>
Modular<P> Modular<P>::get_pow(ValueType power) const {
    if (power == 0) {
        return Modular(1);
    } else if (power < 0) {
        return get_pow(-power).get_inverse();
    } else {
        Modular result = 1;
        Modular current_power = *this;
        while (power > 0) {
            if (power % 2 == 1) {
                result *= current_power;
            }
            power /= 2;
            current_power *= current_power;
        }
        return result;
    }
}

template<auto P>
Modular<P> Modular<P>::get_inverse() const {
    return get_pow(P - 2);
}

template<auto P>
typename Modular<P>::ValueType Modular<P>::get_value() const {
    return value;
}

template<auto P>
std::ostream& operator << (std::ostream& out, const Modular<P>& other) {
    return out << other.value;
}

template<auto P>
std::istream& operator >> (std::istream& in, Modular<P>& other) {
    in >> other.value;
    other.normalize();
    return in;
}

template<auto P>
void Modular<P>::normalize() {
    if (value < 0) {
        value = value % P + P;
    } else if (value >= P) {
        value = value % P;
    }
}

}  // grobner

#endif  // GROBNER_MODULAR_H_
