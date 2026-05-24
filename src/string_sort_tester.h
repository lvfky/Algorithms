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

namespace tester_detail {

inline auto makeCharCounter(int64_t &comps) {
  return [&comps](const std::string &a, const std::string &b) {
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
}

using Time = std::chrono::high_resolution_clock::time_point;

inline Time tick() {
  return std::chrono::high_resolution_clock::now();
}

inline SortResult finish(Time t0, int64_t comps) {
  Time t1 = tick();
  double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
  return {ms, comps};
}

}  // namespace tester_detail

class StringSortTester {
 public:
  SortResult testStdSort(std::vector<std::string> arr) {
    int64_t comps = 0;
    auto cmp = tester_detail::makeCharCounter(comps);
    auto t0 = tester_detail::tick();
    std::sort(arr.begin(), arr.end(), cmp);
    return tester_detail::finish(t0, comps);
  }

  SortResult testStdMergeSort(std::vector<std::string> arr) {
    int64_t comps = 0;
    auto cmp = tester_detail::makeCharCounter(comps);
    auto t0 = tester_detail::tick();
    std::stable_sort(arr.begin(), arr.end(), cmp);
    return tester_detail::finish(t0, comps);
  }

  SortResult testStringQuickSort(std::vector<std::string> arr) {
    int64_t comps = 0;
    auto t0 = tester_detail::tick();
    stringQuickSort(arr, comps);
    return tester_detail::finish(t0, comps);
  }

  SortResult testStringMergeSortLcp(std::vector<std::string> arr) {
    int64_t comps = 0;
    auto t0 = tester_detail::tick();
    stringMergeSortLcp(arr, comps);
    return tester_detail::finish(t0, comps);
  }

  SortResult testMsdRadixSort(std::vector<std::string> arr) {
    int64_t comps = 0;
    auto t0 = tester_detail::tick();
    msdRadixSort(arr, comps);
    return tester_detail::finish(t0, comps);
  }

  SortResult testMsdRadixQuickSort(std::vector<std::string> arr) {
    int64_t comps = 0;
    auto t0 = tester_detail::tick();
    msdRadixQuickSort(arr, comps);
    return tester_detail::finish(t0, comps);
  }
};
