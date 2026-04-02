### 1. 高精度计时器类（Timer）——推荐放在“3.2 计时模块”

```cpp
// include/timer.h
#pragma once
#include <chrono>

class Timer {
public:
    void start();                    // 开始计时并清零比较次数
    double stop();                   // 结束计时，返回毫秒
    long long getComparisonCount() const;   // 获取比较次数
    void resetComparisonCount();     // 重置比较次数
    void incrementComparison();      // 每次元素比较时调用

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    long long comparisonCount = 0;
};
```

```cpp
// src/timer.cpp
#include "timer.h"

void Timer::start() {
    startTime = std::chrono::high_resolution_clock::now();
    comparisonCount = 0;
}

double Timer::stop() {
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - startTime).count();
}

long long Timer::getComparisonCount() const { return comparisonCount; }
void Timer::resetComparisonCount() { comparisonCount = 0; }
void Timer::incrementComparison() { ++comparisonCount; }
```

---

### 2. 快速排序（Quick Sort）核心实现——推荐放在“3.3 排序算法模块”

```cpp
// src/sort_algorithms.cpp （快速排序部分）
void quickSortHelper(std::vector<int>& arr, int low, int high) {
    if (low >= high) return;
    
    int pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; ++j) {
        if (g_timer) g_timer->incrementComparison();   // 统计比较次数
        if (arr[j] < pivot) {
            ++i;
            swap(arr[i], arr[j]);
        }
    }
    if (g_timer) g_timer->incrementComparison();
    swap(arr[i + 1], arr[high]);
    
    int pi = i + 1;
    quickSortHelper(arr, low, pi - 1);
    quickSortHelper(arr, pi + 1, high);
}

void quickSort(std::vector<int>& arr) {
    quickSortHelper(arr, 0, arr.size() - 1);
}
```

---

### 3. 堆排序（Heap Sort）核心实现——推荐放在“3.3 排序算法模块”

```cpp
// src/sort_algorithms.cpp （堆排序部分）
void heapify(std::vector<int>& arr, int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n) {
        if (g_timer) g_timer->incrementComparison();
        if (arr[l] > arr[largest]) largest = l;
    }
    if (r < n) {
        if (g_timer) g_timer->incrementComparison();
        if (arr[r] > arr[largest]) largest = r;
    }

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(std::vector<int>& arr) {
    int n = arr.size();
    // 建堆
    for (int i = n / 2 - 1; i >= 0; --i)
        heapify(arr, n, i);
    // 排序
    for (int i = n - 1; i > 0; --i) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}
```

---

### 4. 主测试流程（main.cpp 核心部分）——推荐放在“3.4 测试主流程”

```cpp
// src/main.cpp （核心测试流程）
int main() {
    std::vector<int> sizes = {100, 1000, 5000, 10000, 50000, 100000};
    std::vector<std::string> algorithms = {"Bubble", "Quick", "Heap", "Shell", "Merge", "Insertion"};
    std::vector<std::string> dataTypes = {"Random", "NearlySorted"};

    Timer globalTimer;
    g_timer = &globalTimer;

    for (const auto& dataType : dataTypes) {
        for (int size : sizes) {
            for (const auto& algo : algorithms) {
                int runs = (algo == "Bubble" && size >= 50000) ? 3 : 8;
                double totalTime = 0.0;
                long long totalComp = 0;

                for (int r = 0; r < runs; ++r) {
                    auto arr = (dataType == "Random") ? 
                               generateRandomInt(size) : generateNearlySortedInt(size);

                    g_timer->resetComparisonCount();
                    g_timer->start();

                    // 根据算法名称调用对应排序函数
                    if (algo == "Bubble")      bubbleSort(arr);
                    else if (algo == "Quick")  quickSort(arr);
                    else if (algo == "Heap")   heapSort(arr);
                    // ... 其他算法类似

                    totalTime += g_timer->stop();
                    totalComp += g_timer->getComparisonCount();
                }
                // 保存到 CSV ...
            }
        }
    }
    return 0;
}
```