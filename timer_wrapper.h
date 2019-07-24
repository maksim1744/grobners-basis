#ifndef GROBNER_TIMER_WRAPPER_H_
#define GROBNER_TIMER_WRAPPER_H_

#include <ctime>
#include <string>

namespace grobner {

template<typename TFunction>
class TimerWrapper {
  public:
    TimerWrapper(TFunction function, std::string message):
        call(function),
        message_(message),
        start_time_(clock()) {
    }

    template<class... TArgs>
    auto operator() (TArgs&&... args) const {
        return call(std::forward<TArgs>(args)...);
    }

    ~TimerWrapper() {
        auto endTime_ = clock();
        auto diff = (endTime_ - start_time_);
        long double elapsed_time = (long double)(diff) / CLOCKS_PER_SEC;
        std::cout << message_ << elapsed_time << std::endl;
    }

    TFunction call;

  private:
    clock_t start_time_;
    std::string message_;
};


template<typename TFunction>
TimerWrapper<TFunction> test_time(TFunction function, const std::string& message) {
    return TimerWrapper<TFunction>(function, message);
}

}  // grobner

#endif  // GROBNER_TIMER_WRAPPER_H_
