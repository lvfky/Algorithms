#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

constexpr int kAlphabet = 256;
constexpr int kRadixQuickThreshold = 74;

int charAt(const std::string &s, int d) {
  if (d < static_cast<int>(s.size())) {
    return static_cast<unsigned char>(s[d]);
  }
  return -1;
}

int charAtSentinel(const std::string &s, int d) {
  if (d < static_cast<int>(s.size())) {
    return static_cast<unsigned char>(s[d]) + 1;
  }
  return 0;
}

void stringQuickSortRec(std::vector<std::string> &arr, int left, int right, int d) {
  if (left >= right) {
    return;
  }
  int lt = left;
  int gt = right;
  int i = left + 1;
  int pivot = charAt(arr[left], d);
  while (i <= gt) {
    int t = charAt(arr[i], d);
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
  stringQuickSortRec(arr, left, lt - 1, d);
  if (pivot >= 0) {
    stringQuickSortRec(arr, lt, gt, d + 1);
  }
  stringQuickSortRec(arr, gt + 1, right, d);
}

void msdRadixQuickRec(std::vector<std::string> &arr, int left, int right, int d,
                      std::vector<std::string> &aux) {
  if (left >= right) {
    return;
  }
  if (right - left + 1 < kRadixQuickThreshold) {
    stringQuickSortRec(arr, left, right, d);
    return;
  }
  std::vector<int> count(kAlphabet + 2, 0);
  for (int i = left; i <= right; ++i) {
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
    msdRadixQuickRec(arr, subLeft, subRight, d + 1, aux);
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int n;
  std::cin >> n;
  if (n <= 0) {
    return 0;
  }
  std::vector<std::string> arr(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> arr[i];
  }
  std::vector<std::string> aux(n);
  msdRadixQuickRec(arr, 0, n - 1, 0, aux);
  for (int i = 0; i < n; ++i) {
    std::cout << arr[i] << "\n";
  }
  return 0;
}