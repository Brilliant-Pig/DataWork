#include "data_generator.h"
#include <random>
#include <ctime>

// 随机整数生成器
std::vector<int> generateRandomInt(int n, int minVal, int maxVal) {
    std::vector<int> arr(n);
    std::mt19937 gen(static_cast<unsigned>(time(nullptr) + n));  // 加入n避免重复
    std::uniform_int_distribution<> dis(minVal, maxVal);
    for (int i = 0; i < n; ++i) {
        arr[i] = dis(gen);
    }
    return arr;
}

// 几乎有序数组生成器（90%有序，少量乱序）
std::vector<int> generateNearlySortedInt(int n) {
    std::vector<int> arr(n);
    for (int i = 0; i < n; ++i) {
        arr[i] = i;                    // 先完全有序
    }
    
    // 随机打乱约 10% 的元素，制造“几乎有序”效果
    std::mt19937 gen(static_cast<unsigned>(time(nullptr) + n * 10));
    std::uniform_int_distribution<> dis(0, n - 1);
    
    int swaps = n / 10;   // 打乱约10%的位置
    for (int i = 0; i < swaps; ++i) {
        int idx1 = dis(gen);
        int idx2 = dis(gen);
        std::swap(arr[idx1], arr[idx2]);
    }
    return arr;
}