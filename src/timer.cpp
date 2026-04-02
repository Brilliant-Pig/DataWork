#include "timer.h"

void Timer::start() {
    startTime = std::chrono::high_resolution_clock::now();
    comparisonCount = 0;
}

double Timer::stop() {
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - startTime).count();
}