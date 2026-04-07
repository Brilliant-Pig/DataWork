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

实验结果表明，传统排序算法在不同数据分布下性能差异显著。冒泡排序在大规模随机数据下效率极低，快速排序在随机数据中表现较好但最坏情况下容易退化，插入排序在几乎有序数据中优势明显，而其他算法也各有局限性。

针对这些问题，本研究对快速排序和冒泡排序进行了针对性改进，其余算法（堆排序、希尔排序、归并排序、插入排序）保持原有标准实现。

#### 3.3.1 快速排序的改进

快速排序的性能很大程度上取决于枢轴的选择。若每次都选取序列最后一个元素作为枢轴，在接近有序的数据中极易产生最坏情况。本研究采用**三数取中法**（Median-of-Three）来选择枢轴，即从左端、中间和右端三个位置选取元素，取其中间值作为枢轴。该改进能有效避免最坏情况，提高算法的稳定性和平均性能。

改进后的快速排序划分函数核心代码如下：

```cpp
// 改进后的快速排序划分函数（采用三数取中法）
int partition(std::vector<int>& arr, int low, int high) {
    int mid = low + (high - low) / 2;
    
    // 三数取中
    if (arr[low] > arr[mid]) swap(arr[low], arr[mid]);
    if (arr[low] > arr[high]) swap(arr[low], arr[high]);
    if (arr[mid] > arr[high]) swap(arr[mid], arr[high]);
    
    // 将中间值交换到 high 位置作为枢轴
    swap(arr[mid], arr[high]);
    
    int pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; ++j) {
        if (g_timer) g_timer->incrementComparison();
        if (arr[j] < pivot) {
            ++i;
            swap(arr[i], arr[j]);
        }
    }
    if (g_timer) g_timer->incrementComparison();
    swap(arr[i + 1], arr[high]);
    return i + 1;
}
```

#### 3.3.2 冒泡排序的改进

冒泡排序在数据已经有序或接近有序时仍会进行不必要的完整遍历。本研究为其增加了**有序标志位**（flag），若某一趟比较过程中没有发生交换，则说明序列已有序，可提前终止排序。该改进在几乎有序数据场景下能显著减少不必要的比较。

改进后的冒泡排序代码如下：

```cpp
// 改进后的冒泡排序（增加有序标志位提前终止）
void bubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;          // 有序标志位
        for (int j = 0; j < n - i - 1; ++j) {
            if (g_timer) g_timer->incrementComparison();
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;           // 若本趟无交换，则已有序，提前结束
    }
}
```

通过上述两处改进，本研究在保持原有算法框架不变的前提下，提升了快速排序的稳定性和冒泡排序在近乎有序数据下的效率。实验数据也表明，改进后的算法在随机数据和几乎有序数据场景下均取得了更好的综合表现。

---