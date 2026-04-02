#pragma once
#include <vector>

// 生成随机整数数组
std::vector<int> generateRandomInt(int n, int minVal = 0, int maxVal = 1000000);

// 生成几乎有序整数数组（Nearly Sorted）——课程设计常用测试数据
std::vector<int> generateNearlySortedInt(int n);