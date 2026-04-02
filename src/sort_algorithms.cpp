#include "sort_algorithms.h"
#include "timer.h"   // 需要使用 Timer 的比较计数功能

// 全局或外部传入 Timer 指针来统计比较次数
extern Timer* g_timer;   // 我们会在 main 中设置

void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

// ====================== 冒泡排序 ======================
void bubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (g_timer) g_timer->incrementComparison();   // 统计比较次数
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// ====================== 快速排序 ======================
void quickSortHelper(std::vector<int>& arr, int low, int high) {
    if (low >= high) return;
    
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
    
    int pi = i + 1;
    quickSortHelper(arr, low, pi - 1);
    quickSortHelper(arr, pi + 1, high);
}

void quickSort(std::vector<int>& arr) {
    quickSortHelper(arr, 0, arr.size() - 1);
}

// ====================== 堆排序 ======================
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
    for (int i = n / 2 - 1; i >= 0; --i)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; --i) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// ====================== 希尔排序 ======================
void shellSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; ++i) {
            int temp = arr[i];
            int j = i;
            while (j >= gap) {
                if (g_timer) g_timer->incrementComparison();
                if (arr[j - gap] > temp) {
                    arr[j] = arr[j - gap];
                    j -= gap;
                } else {
                    break;
                }
            }
            arr[j] = temp;
        }
    }
}

// ====================== 归并排序 ======================
void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
    for (int i = 0; i < n2; ++i) R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (g_timer) g_timer->incrementComparison();
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSortHelper(std::vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSortHelper(arr, left, mid);
    mergeSortHelper(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

void mergeSort(std::vector<int>& arr) {
    mergeSortHelper(arr, 0, arr.size() - 1);
}

// ====================== 插入排序 ======================
void insertionSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0) {
            if (g_timer) g_timer->incrementComparison();
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                --j;
            } else {
                break;
            }
        }
        arr[j + 1] = key;
    }
}