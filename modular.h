#ifndef GROBNER_MODULAR_H_
#define GROBNER_MODULAR_H_

#include <iostream>

namespace grobner {

// some methods work correctly only with prime P (get_inverse, division)
template<auto P>
class Modular {
  public:
    using ValueType = decltype(P);

    Modular(ValueType value = 0);

    static ValueType make_normal(ValueType value);

    Modular get_pow(ValueType power) const;
    Modular get_inverse() const;
    ValueType get_value() const;

    template<auto Q>
    friend std::ostream& operator << (std::ostream& out, const Modular<Q>& other);
    template<auto Q>
    friend std::istream& operator >> (std::istream& in, Modular<Q>& other);

    Modular operator - () const;

    friend Modular& operator += (Modular& first, const Modular& second) {
        first.value_ += second.value_;
        if (first.value_ >= P) {
            first.value_ -= P;
        }
        return first;
    }
    friend Modular operator + (const Modular& first, const Modular& second) {
        auto result = first;
        return result += second;
    }
    friend Modular& operator -= (Modular& first, const Modular& second) {
        first.value_ -= second.value_;
        if (first.value_ < 0) {
            first.value_ += P;
        }
        return first;
    }
    friend Modular operator - (const Modular& first, const Modular& second) {
        auto result = first;
        return result -= second;
    }
    friend Modular& operator *= (Modular& first, const Modular& second) {
        first.value_ = first.value_ * second.value_ % P;
        return first;
    }
    friend Modular operator * (const Modular& first, const Modular& second) {
        auto result = first;
        return result *= second;
    }
    friend Modular& operator /= (Modular& first, const Modular& second) {
        first *= second.get_inverse();
        return first;
    }
    friend Modular operator / (const Modular& first, const Modular& second) {
        auto result = first;
        return result /= second;
    }

    friend bool operator == (const Modular& first, const Modular& second) {
        return first.value_ == second.value_;
    }
    friend bool operator != (const Modular& first, const Modular& second) {
        return !(first == second);
    }

  private:
    ValueType value_;

    void normalize();
};

template<auto P>
Modular<P>::Modular(typename Modular<P>::ValueType value) : value_(make_normal(value)) {}

template<auto P>
typename Modular<P>::ValueType Modular<P>::make_normal(typename Modular<P>::ValueType value) {
    value %= P;
    if (value < 0) {
        value += P;
    }
    return value;
}

template<auto P>
Modular<P> Modular<P>::operator - () const {
    return Modular<P>(-value_);
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
    return value_;
}

template<auto P>
std::ostream& operator << (std::ostream& out, const Modular<P>& other) {
    return out << other.value_;
}

template<auto P>
std::istream& operator >> (std::istream& in, Modular<P>& other) {
    typename Modular<P>::ValueType tmp;
    in >> tmp;
    other = tmp;
    return in;
}


template<auto P>
void Modular<P>::normalize() {
    value_ = make_normal(value_);
}

}  // grobner

#endif  // GROBNER_MODULAR_H_
