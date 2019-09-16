#ifndef GROBNER_PAIR_SET_ITERATOR_H_
#define GROBNER_PAIR_SET_ITERATOR_H_

#include <utility>

namespace grobner {

template<class ElemIt>
class PairSetIterator {
  public:
    PairSetIterator(ElemIt first, ElemIt second, ElemIt c_begin, ElemIt c_end);

    PairSetIterator<ElemIt> operator++();

    auto operator * ();

    auto first();
    auto second();

    bool operator != (const PairSetIterator& other);

  private:
    ElemIt first_;
    ElemIt second_;
    ElemIt c_begin_;
    ElemIt c_end_;
};


template<class ElemIt>
PairSetIterator<ElemIt>::PairSetIterator(ElemIt first, ElemIt second, ElemIt c_begin, ElemIt c_end) :
    first_(first),
    second_(second),
    c_begin_(c_begin),
    c_end_(c_end) {
}

template<class ElemIt>
PairSetIterator<ElemIt> PairSetIterator<ElemIt>::operator++() {
    ++second_;
    if (first_ == second_) {
        ++first_;
        second_ = c_begin_;
    }
    return *this;
}

template<class ElemIt>
auto PairSetIterator<ElemIt>::operator * () {
    return std::make_pair(*first_, *second_);
}

template<class ElemIt>
auto PairSetIterator<ElemIt>::first() {
    return *first_;
}

template<class ElemIt>
auto PairSetIterator<ElemIt>::second() {
    return *second_;
}

template<class ElemIt>
bool PairSetIterator<ElemIt>::operator != (const PairSetIterator& other) {
    return first_ != other.first_ || second_ != other.second_;
}

}  // grobner

#endif  // GROBNER_PAIR_SET_ITERATOR_H_
