#pragma once
#include <chrono>

class Timer {
public:
    void start();
    double stop();           // 返回毫秒
    long long getComparisonCount() const { return comparisonCount; }
    void resetComparisonCount() { comparisonCount = 0; }
    void incrementComparison() { ++comparisonCount; }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    long long comparisonCount = 0;
};