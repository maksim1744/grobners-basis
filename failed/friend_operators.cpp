template<auto P>
class Modular {
  public:
    using ValueType = decltype(P);

    ValueType norm(ValueType value) {
        return (value % P + P) % P;
    }

    Modular(ValueType value) {
        value_ = norm(value);
    }

    template<auto Q>
    friend Modular<Q> operator + (const Modular<Q>& first, const Modular<Q>& second);

    ValueType value_;
};

template<auto Q>
Modular<Q> operator + (const Modular<Q>& first, const Modular<Q>& second) {
    return Modular<Q>(first.value_ + second.value_);
}

using Mod = Modular<17>;

int main() {
    auto a = Mod(10) + Mod(15); // fine
    auto b = Mod(10) + 15;  // error: no match for 'operator+' (operand types are 'Modular<17>' and 'int')
                            // note: mismatched types 'const Modular<Q>' and 'int'
    auto c = 10 + Mod(15);  // error: no match for 'operator+' (operand types are 'int' and 'Modular<17>')
                            // note: mismatched types 'const Modular<Q>' and 'int'
    return 0;
}
