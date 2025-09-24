#include <chrono>

namespace ch = std::chrono;

class Timer {
    ch::steady_clock::time_point _start;
public:
    Timer() = default;

    void start();
    double getTime() const;
};