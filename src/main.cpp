#include "sort_algorithms.h"
#include "data_generator.h"
#include "timer.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

// 全局 Timer 指针，用于统计比较次数
Timer* g_timer = nullptr;

int main() {
    // 测试规模
    std::vector<int> sizes = {100, 1000, 5000, 10000, 50000, 100000};
    
    // 算法名称
    std::vector<std::string> algorithms = {"Bubble", "Quick", "Heap", "Shell", "Merge", "Insertion"};
    
    // 数据类型：随机数据 和 几乎有序数据
    std::vector<std::string> dataTypes = {"Random", "NearlySorted"};
    
    // CSV 表头
    std::vector<std::vector<std::string>> csvData = {
        {"Algorithm", "DataType", "Size", "AvgTime(ms)", "AvgComparisons"}
    };

    std::cout << "========== 数据结构课程设计 - 排序算法效率比较 ==========\n";
    std::cout << "测试环境：Ubuntu + GCC 13.3.0\n\n";

    Timer globalTimer;
    g_timer = &globalTimer;

    for (const auto& dataType : dataTypes) {
        std::cout << "【" << dataType << " 数据测试】\n";
        std::cout << "--------------------------------------------------\n";

        for (int size : sizes) {
            std::cout << "数据规模 n = " << size << "\n";
            
            for (const auto& algoName : algorithms) {
                double totalTime = 0.0;
                long long totalComparisons = 0;
                
                // Bubble Sort 在大数组时减少测试次数，避免运行过久
                int runs = (algoName == "Bubble" && size >= 50000) ? 3 : 8;

                for (int r = 0; r < runs; ++r) {
                    // 生成测试数据
                    auto arr = (dataType == "Random") ? 
                               generateRandomInt(size) : 
                               generateNearlySortedInt(size);

                    // 重置计时器和比较次数
                    g_timer->resetComparisonCount();
                    g_timer->start();

                    // 执行排序
                    if (algoName == "Bubble")      bubbleSort(arr);
                    else if (algoName == "Quick")  quickSort(arr);
                    else if (algoName == "Heap")   heapSort(arr);
                    else if (algoName == "Shell")  shellSort(arr);
                    else if (algoName == "Merge")  mergeSort(arr);
                    else if (algoName == "Insertion") insertionSort(arr);

                    double elapsed = g_timer->stop();
                    totalTime += elapsed;
                    totalComparisons += g_timer->getComparisonCount();
                }

                double avgTime = totalTime / runs;
                double avgComp = static_cast<double>(totalComparisons) / runs;

                // 保存到 CSV
                csvData.push_back({
                    algoName,
                    dataType,
                    std::to_string(size),
                    std::to_string(avgTime),
                    std::to_string(static_cast<long long>(avgComp))
                });

                // 控制台输出
                std::cout << std::left << std::setw(12) << algoName
                          << "  时间: " << std::fixed << std::setprecision(3) << avgTime << " ms"
                          << "   比较次数: " << static_cast<long long>(avgComp) << "\n";
            }
            std::cout << "\n";
        }
    }

    // 保存 CSV 文件
    writeCSV("../results/sort_results.csv", csvData);
    
    std::cout << "✅ 测试全部完成！\n";
    std::cout << "   结果文件已保存到：results/sort_results.csv\n";
    std::cout << "   文件包含：算法名称、数据类型、数据规模、平均时间(ms)、平均比较次数\n\n";
    
    std::cout << "提示：可以用 Excel 打开 CSV 文件绘制性能对比折线图。\n";

    return 0;
}