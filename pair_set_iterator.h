#ifndef GROBNER_PAIR_SET_ITERATOR_H_
#define GROBNER_PAIR_SET_ITERATOR_H_

namespace grobner {

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

}  // grobner

#endif  // GROBNER_PAIR_SET_ITERATOR_H_
