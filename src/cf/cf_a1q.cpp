#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

int charAt(const std::string &s, int d) {
  if (d < static_cast<int>(s.size())) {
    return static_cast<unsigned char>(s[d]);
  }
  return -1;
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
  stringQuickSortRec(arr, 0, n - 1, 0);
  for (int i = 0; i < n; ++i) {
    std::cout << arr[i] << "\n";
  }
  return 0;
}
