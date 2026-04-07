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
---
**以下是严格按照课程设计报告规范撰写的“改进算法”部分**，语言正式、客观，使用“本研究”“本文”“实验表明”等学术常用表达，避免了第一人称、第二人称和过于口语化的表述。

---

### 5 改进算法

实验结果表明，传统排序算法在不同数据分布下存在明显的性能差异。冒泡排序在随机数据场景中效率最低，当数据规模达到\( n=100000 \)时，平均运行时间为28106.01 ms，元素比较次数接近50亿次，充分暴露了\( O(n^2) \)时间复杂度的缺陷。快速排序在随机数据下表现最佳，但在几乎有序数据中优势有所减弱；插入排序则在几乎有序数据中优势显著，但在完全随机数据下性能急剧下降。堆排序、希尔排序和归并排序虽然整体表现较好，但在数据特征未知的情况下难以始终保持最优性能。

由此可见，单一固定算法难以同时适应随机数据和几乎有序数据这两种典型场景，这是现有排序算法在实际应用中的主要不足。

针对上述问题，本研究提出一种**基于数据特征的自适应混合排序算法**（Adaptive Hybrid Sorting）。该算法的核心思想是在排序前通过少量采样快速判断数据的有序程度，再根据判断结果动态选择最适合的基础算法，从而实现算法与数据特征的自动匹配。

本算法的具体改进内容如下：

首先，在排序开始前增加一个有序度检测步骤。该步骤通过采样部分元素进行比较，估算数据的有序程度。若有序度达到85%以上，则判定为“几乎有序”；否则判定为“随机分布”。

其次，根据检测结果和数据规模采用不同的排序策略：当数据规模\( n \leq 1000 \)时，直接采用插入排序；当数据为几乎有序时，优先选用插入排序以发挥其在近乎有序数据下的线性优势；当数据为随机分布时，则优先选用快速排序以获得\( O(n \log n) \)的平均最优性能。

改进后的算法流程可描述为：

```pseudocode
算法：AdaptiveHybridSort(arr)
输入：待排序数组 arr
输出：已排序数组

1. n ← arr.length
2. 若 n ≤ 1000，则调用 InsertionSort(arr) 并返回
3. orderedDegree ← CalculateOrderDegree(arr)   // 通过采样计算有序度
4. 若 orderedDegree ≥ 0.85，则
       调用 InsertionSort(arr)
   否则
       调用 QuickSort(arr)
5. 返回 arr
```

其中，`CalculateOrderDegree`函数采用采样策略，仅需少量比较即可完成检测，时间开销可忽略不计。原有6种排序算法的核心实现均未做改动，仅通过全局计时器统计比较次数。

实验数据验证了该改进思路的有效性：在几乎有序数据下，插入排序的性能远优于快速排序；而在随机数据下，快速排序又能发挥最大优势。通过动态选择机制，本改进算法在混合数据场景下可获得更好的综合性能。

---