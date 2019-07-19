#ifndef GROBNER_TIMER_WRAPPER_H_
#define GROBNER_TIMER_WRAPPER_H_

#include <ctime>

namespace grobner {

template <typename TFunction>
class TimerWrapper {
  public:
    TimerWrapper(TFunction function, clock_t& elapsed_time):
        call(function),
        start_time_(clock()),
        elapsed_time_(elapsed_time) {
    }

    ~TimerWrapper() {
        const clock_t endTime_ = ::clock();
        const clock_t diff = (endTime_ - start_time_);
        elapsed_time_ += diff;
    }

    TFunction call;

  private:
    const clock_t start_time_;
    clock_t& elapsed_time_;
};


template <typename TFunction>
TimerWrapper<TFunction> test_time(TFunction function, clock_t& elapsed_time) {
    return TimerWrapper<TFunction>(function, elapsed_time);
}

}  // grobner

#endif  // GROBNER_TIMER_WRAPPER_H_
