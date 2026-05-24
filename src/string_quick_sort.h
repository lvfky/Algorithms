#pragma once

#include <cstdint>
#include <string>
#include <vector>

inline int charAt(const std::string &s, int d) {
  if (d < static_cast<int>(s.size())) {
    return static_cast<unsigned char>(s[d]);
  }
  return -1;
}

inline void stringQuickSortRec(std::vector<std::string> &arr, int left, int right, int d, int64_t &comps) {
  if (left >= right) {
    return;
  }
  int lt = left;
  int gt = right;
  int i = left + 1;
  int pivot = charAt(arr[left], d);
  while (i <= gt) {
    int t = charAt(arr[i], d);
    ++comps;
    if (t < pivot) {
      std::swap(arr[lt], arr[i]);
      ++lt;
      ++i;
    } else if (t > pivot) {
      std::swap(arr[i], arr[gt]);
      --gt;
    } else {
      ++i;
    }
  }
  stringQuickSortRec(arr, left, lt - 1, d, comps);
  if (pivot >= 0) {
    stringQuickSortRec(arr, lt, gt, d + 1, comps);
  }
  stringQuickSortRec(arr, gt + 1, right, d, comps);
}

inline void stringQuickSort(std::vector<std::string> &arr, int64_t &comps) {
  int n = static_cast<int>(arr.size());
  if (n <= 1) {
    return;
  }
  stringQuickSortRec(arr, 0, n - 1, 0, comps);
}
