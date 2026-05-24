#pragma once

#include <cstdint>
#include <string>
#include <vector>

inline constexpr int kAlphabet = 256;

inline int charAtSentinel(const std::string &s, int d) {
  if (d < static_cast<int>(s.size())) {
    return static_cast<unsigned char>(s[d]) + 1;
  }
  return 0;
}

inline void msdRadixRec(std::vector<std::string> &arr, int left, int right, int d,
                        std::vector<std::string> &aux, int64_t &comps) {
  if (left >= right) {
    return;
  }
  std::vector<int> count(kAlphabet + 2, 0);
  for (int i = left; i <= right; ++i) {
    ++comps;
    int c = charAtSentinel(arr[i], d);
    ++count[c + 1];
  }
  for (int i = 0; i < kAlphabet + 1; ++i) {
    count[i + 1] += count[i];
  }
  for (int i = left; i <= right; ++i) {
    int c = charAtSentinel(arr[i], d);
    aux[left + count[c]] = std::move(arr[i]);
    ++count[c];
  }
  for (int i = left; i <= right; ++i) {
    arr[i] = std::move(aux[i]);
  }
  for (int c = 1; c <= kAlphabet; ++c) {
    int subLeft = left + count[c - 1];
    int subRight = left + count[c] - 1;
    msdRadixRec(arr, subLeft, subRight, d + 1, aux, comps);
  }
}

inline void msdRadixSort(std::vector<std::string> &arr, int64_t &comps) {
  int n = static_cast<int>(arr.size());
  if (n <= 1) {
    return;
  }
  std::vector<std::string> aux(n);
  msdRadixRec(arr, 0, n - 1, 0, aux, comps);
}
