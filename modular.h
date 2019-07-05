#ifndef GROBNER_MODULAR_H_
#define GROBNER_MODULAR_H_

#include <iostream>

namespace grobner {

// some methods work correctly only with prime P (get_inverse, division)
template<class ValueType, ValueType P>
class Modular {
  public:

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

    template<class OtherValueType, OtherValueType Q>
    friend std::ostream& operator << (std::ostream& out, const Modular<OtherValueType, Q>& other);
    template<class OtherValueType, OtherValueType Q>
    friend std::istream& operator >> (std::istream& in, Modular<OtherValueType, Q>& other);

  private:
    ValueType value;
    void normalize();
};

template<class ValueType, ValueType P>
Modular<ValueType, P>::Modular(ValueType _value) : value(_value) {
    normalize();
}

template<class ValueType, ValueType P>
Modular<ValueType, P>& Modular<ValueType, P>::operator += (const Modular& other) {
    value += other.value;
    if (value >= P) {
        value -= P;
    }
    return *this;
}

template<class ValueType, ValueType P>
Modular<ValueType, P> Modular<ValueType, P>::operator + (const Modular& other) const {
    Modular<ValueType, P> result = *this;
    return result += other;
}

template<class ValueType, ValueType P>
Modular<ValueType, P>& Modular<ValueType, P>::operator -= (const Modular& other) {
    value -= other.value;
    if (value < 0) {
        value += P;
    }
    return *this;
}

template<class ValueType, ValueType P>
Modular<ValueType, P> Modular<ValueType, P>::operator - (const Modular& other) const {
    Modular<ValueType, P> result = *this;
    return result -= other;
}

template<class ValueType, ValueType P>
Modular<ValueType, P>& Modular<ValueType, P>::operator *= (const Modular& other) {
    value = value * other.value % P;
    return *this;
}

template<class ValueType, ValueType P>
Modular<ValueType, P> Modular<ValueType, P>::operator * (const Modular& other) const {
    Modular<ValueType, P> result = *this;
    return result *= other;
}

template<class ValueType, ValueType P>
Modular<ValueType, P>& Modular<ValueType, P>::operator /= (const Modular& other) {
    (*this) *= other.get_inverse();
    return *this;
}

template<class ValueType, ValueType P>
Modular<ValueType, P> Modular<ValueType, P>::operator / (const Modular& other) const {
    Modular<ValueType, P> result = *this;
    return result /= other;
}

template<class ValueType, ValueType P>
Modular<ValueType, P> Modular<ValueType, P>::operator - () const {
    return Modular(0) - (*this);
}

template<class ValueType, ValueType P>
bool Modular<ValueType, P>::operator == (const Modular& other) const {
    return value == other.value;
}

template<class ValueType, ValueType P>
bool Modular<ValueType, P>::operator != (const Modular& other) const {
    return value != other.value;
}


template<class ValueType, ValueType P>
Modular<ValueType, P> Modular<ValueType, P>::get_pow(ValueType power) const {
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

template<class ValueType, ValueType P>
Modular<ValueType, P> Modular<ValueType, P>::get_inverse() const {
    return get_pow(P - 2);
}

template<class ValueType, ValueType P>
ValueType Modular<ValueType, P>::get_value() const {
    return value;
}

template<class ValueType, ValueType P>
std::ostream& operator << (std::ostream& out, const Modular<ValueType, P>& other) {
    return out << other.value;
}

template<class ValueType, ValueType P>
std::istream& operator >> (std::istream& in, Modular<ValueType, P>& other) {
    in >> other.value;
    other.normalize();
    return in;
}

template<class ValueType, ValueType P>
void Modular<ValueType, P>::normalize() {
    if (value < 0) {
        value = value % P + P;
    } else if (value >= P) {
        value = value % P;
    }
}

}  // grobner

#endif  // GROBNER_MODULAR_H_
