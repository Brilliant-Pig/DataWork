# 数据结构课程设计 - 排序算法效率比较（课题31）

**题目**：给出一组实验来比较下列排序算法的时间性能  
**算法**：快速排序、堆排序、希尔排序、冒泡排序、归并排序、插入排序  

本项目实现了 6 种经典排序算法的效率对比实验，支持**随机数据**和**几乎有序数据**两种典型场景，同时统计**运行时间**和**比较次数**，结果自动生成 CSV 文件，方便绘制图表。

---

## 硬件配置

- **GPU**：NVIDIA GeForce RTX 4090
- **CPU**：高性能台式机（HP OMEN 45L）
- **操作系统**：Ubuntu 24.04 LTS
- **内存**：充足
- **编译器**：GCC 13.3.0 + CMake 3.28.3

> 注意：本程序为纯 CPU 计算，RTX 4090 主要提供整体系统性能保障。

---

## 项目文件结构说明

| 路径                        | 文件/文件夹                  | 作用说明 |
|-----------------------------|------------------------------|----------|
| `CMakeLists.txt`            | CMake 配置文件               | 定义项目构建规则，指定 C++17 标准、输出路径等 |
| `include/`                  | 头文件目录                   | 存放所有 `.h` 头文件 |
| `include/sort_algorithms.h` | 排序算法声明                 | 声明 6 种排序函数接口 |
| `include/data_generator.h`  | 数据生成器声明               | 声明随机数据和几乎有序数据生成函数 |
| `include/timer.h`           | 高精度计时器声明             | 提供毫秒级计时 + 比较次数统计功能 |
| `include/utils.h`           | 工具函数声明                 | 提供 CSV 写入功能 |
| `src/`                      | 源代码目录                   | 存放所有 `.cpp` 实现文件 |
| `src/main.cpp`              | **主程序入口**               | 控制整个测试流程、调用排序算法、输出结果、生成 CSV |
| `src/sort_algorithms.cpp`   | **排序算法核心实现**         | 实现 6 种排序算法，并通过全局 Timer 统计比较次数 |
| `src/data_generator.cpp`    | 数据生成器实现               | 生成随机整数数组和几乎有序数组 |
| `src/timer.cpp`             | 计时器实现                   | 高精度计时 + 比较次数计数 |
| `src/utils.cpp`             | 工具函数实现                 | 将实验结果写入 CSV 文件 |
| `build/`                    | 编译输出目录                 | CMake 生成的可执行文件 `SortCompare` 所在位置 |
| `results/`                  | 实验结果目录                 | 存放生成的 `sort_results.csv` |
| `results/sort_results.csv`  | **实验最终结果**             | 包含 72 条完整数据（算法、数据类型、规模、时间、比较次数） |
| `README.md`                 | 本说明文档                   | 项目使用说明和报告参考 |

---

## 如何编译与运行

### 1. 进入项目目录
```bash
cd ~/SortCompare
```
### 2. 编译项目
```bash
cd build
make -j2     # 使用 2 核加速编译
```
### 3. 运行程序
```bash
cd ~/SortCompare/build
./SortCompare
```
推荐一条命令直接运行：
```bash
cd ~/SortCompare/build && ./SortCompare
```
## 运行结果示例（实际输出）
程序运行后会显示：
```txt
========== 数据结构课程设计 - 排序算法效率比较 ==========
测试环境：Ubuntu + GCC 13.3.0

【Random 数据测试】
数据规模 n = 100000
Bubble        时间: 28106.011 ms   比较次数: 4999950000
Quick         时间: 10.434 ms     比较次数: 2027089
Heap          时间: 21.086 ms     比较次数: 3019803
...

【NearlySorted 数据测试】
Insertion     时间: 1547.180 ms   比较次数: 577458939
Quick         时间: 17.933 ms     比较次数: 5546344
...
✅ 测试全部完成！
   结果文件已保存到：results/sort_results.csv
```
**✅ 好的！**  

以下是**专门为报告准备的4段关键代码片段**（已加详细中文注释、格式规范），直接复制到Word的“**详细设计**”和“**附录**”章节即可。  
这些片段精简、重点突出，能很好地体现你作为代码手的贡献（占详细设计40%以上篇幅）。

---

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