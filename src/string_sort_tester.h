#pragma once

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

#include "merge_sort_lcp.h"
#include "msd_radix_quick_sort.h"
#include "msd_radix_sort.h"
#include "string_quick_sort.h"

struct SortResult {
  double timeMs;
  int64_t comparisons;
};

class StringSortTester {
 public:
  SortResult testStdSort(std::vector<std::string> arr) {
    int64_t comps = 0;
    auto cmp = [&comps](const std::string &a, const std::string &b) {
      int la = static_cast<int>(a.size());
      int lb = static_cast<int>(b.size());
      int n = std::min(la, lb);
      for (int i = 0; i < n; ++i) {
        ++comps;
        if (a[i] != b[i]) {
          return a[i] < b[i];
        }
      }
      return la < lb;
    };
    auto start = std::chrono::high_resolution_clock::now();
    std::sort(arr.begin(), arr.end(), cmp);
    auto end = std::chrono::high_resolution_clock::now();
    return {std::chrono::duration<double, std::milli>(end - start).count(), comps};
  }

  SortResult testStdMergeSort(std::vector<std::string> arr) {
    int64_t comps = 0;
    auto cmp = [&comps](const std::string &a, const std::string &b) {
      int la = static_cast<int>(a.size());
      int lb = static_cast<int>(b.size());
      int n = std::min(la, lb);
      for (int i = 0; i < n; ++i) {
        ++comps;
        if (a[i] != b[i]) {
          return a[i] < b[i];
        }
      }
      return la < lb;
    };
    auto start = std::chrono::high_resolution_clock::now();
    std::stable_sort(arr.begin(), arr.end(), cmp);
    auto end = std::chrono::high_resolution_clock::now();
    return {std::chrono::duration<double, std::milli>(end - start).count(), comps};
  }

  SortResult testStringQuickSort(std::vector<std::string> arr) {
    int64_t comps = 0;
    auto start = std::chrono::high_resolution_clock::now();
    stringQuickSort(arr, comps);
    auto end = std::chrono::high_resolution_clock::now();
    return {std::chrono::duration<double, std::milli>(end - start).count(), comps};
  }

  SortResult testStringMergeSortLcp(std::vector<std::string> arr) {
    int64_t comps = 0;
    auto start = std::chrono::high_resolution_clock::now();
    stringMergeSortLcp(arr, comps);
    auto end = std::chrono::high_resolution_clock::now();
    return {std::chrono::duration<double, std::milli>(end - start).count(), comps};
  }

  SortResult testMsdRadixSort(std::vector<std::string> arr) {
    int64_t comps = 0;
    auto start = std::chrono::high_resolution_clock::now();
    msdRadixSort(arr, comps);
    auto end = std::chrono::high_resolution_clock::now();
    return {std::chrono::duration<double, std::milli>(end - start).count(), comps};
  }

  SortResult testMsdRadixQuickSort(std::vector<std::string> arr) {
    int64_t comps = 0;
    auto start = std::chrono::high_resolution_clock::now();
    msdRadixQuickSort(arr, comps);
    auto end = std::chrono::high_resolution_clock::now();
    return {std::chrono::duration<double, std::milli>(end - start).count(), comps};
  }
};
