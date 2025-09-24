#include "timer.hpp"

void Timer::start() {
    _start = std::chrono::steady_clock::now();
}

double Timer::getTime() const {
    auto now = ch::steady_clock::now();

    return static_cast<double>(ch::duration_cast<ch::milliseconds>(now - _start).count()) / 1000.0;
}
