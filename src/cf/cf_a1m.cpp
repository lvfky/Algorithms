#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

struct StrEntry {
  std::string str;
  int lcp;
};

int lcpCompareFrom(const std::string &a, const std::string &b, int &pos) {
  int la = static_cast<int>(a.size());
  int lb = static_cast<int>(b.size());
  while (pos < la && pos < lb) {
    if (a[pos] != b[pos]) {
      return (a[pos] < b[pos]) ? -1 : 1;
    }
    ++pos;
  }
  if (la == lb) {
    return 0;
  }
  return (la < lb) ? -1 : 1;
}

void mergeLcp(std::vector<StrEntry> &arr, int left, int mid, int right) {
  int n1 = mid - left + 1;
  int n2 = right - mid;
  std::vector<StrEntry> leftRun(arr.begin() + left, arr.begin() + mid + 1);
  std::vector<StrEntry> rightRun(arr.begin() + mid + 1, arr.begin() + right + 1);
  leftRun[0].lcp = 0;
  rightRun[0].lcp = 0;

  int i = 0;
  int j = 0;
  int k = left;
  int h = 0;
  int cmp = lcpCompareFrom(leftRun[0].str, rightRun[0].str, h);
  int lastFrom = 0;
  int crossLcp = 0;

  while (i < n1 && j < n2) {
    if (cmp <= 0) {
      arr[k] = leftRun[i];
      if (lastFrom == -1) {
        arr[k].lcp = leftRun[i].lcp;
      } else if (lastFrom == 1) {
        arr[k].lcp = crossLcp;
      } else {
        arr[k].lcp = 0;
      }
      ++k;
      ++i;
      lastFrom = -1;
      if (i >= n1) {
        crossLcp = h;
        break;
      }
      int stored = leftRun[i].lcp;
      crossLcp = h;
      if (stored < h) {
        h = stored;
        cmp = 1;
      } else if (stored > h) {
        cmp = -1;
      } else {
        cmp = lcpCompareFrom(leftRun[i].str, rightRun[j].str, h);
      }
    } else {
      arr[k] = rightRun[j];
      if (lastFrom == 1) {
        arr[k].lcp = rightRun[j].lcp;
      } else if (lastFrom == -1) {
        arr[k].lcp = crossLcp;
      } else {
        arr[k].lcp = 0;
      }
      ++k;
      ++j;
      lastFrom = 1;
      if (j >= n2) {
        crossLcp = h;
        break;
      }
      int stored = rightRun[j].lcp;
      crossLcp = h;
      if (stored < h) {
        h = stored;
        cmp = -1;
      } else if (stored > h) {
        cmp = 1;
      } else {
        cmp = lcpCompareFrom(leftRun[i].str, rightRun[j].str, h);
      }
    }
  }
  while (i < n1) {
    arr[k] = leftRun[i];
    if (lastFrom == 1) {
      arr[k].lcp = crossLcp;
    } else {
      arr[k].lcp = leftRun[i].lcp;
    }
    lastFrom = -1;
    ++k;
    ++i;
  }
  while (j < n2) {
    arr[k] = rightRun[j];
    if (lastFrom == -1) {
      arr[k].lcp = crossLcp;
    } else {
      arr[k].lcp = rightRun[j].lcp;
    }
    lastFrom = 1;
    ++k;
    ++j;
  }
}

void mergeSortLcpRec(std::vector<StrEntry> &arr, int left, int right) {
  if (left >= right) {
    return;
  }
  int mid = (left + right) / 2;
  mergeSortLcpRec(arr, left, mid);
  mergeSortLcpRec(arr, mid + 1, right);
  mergeLcp(arr, left, mid, right);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int n;
  std::cin >> n;
  if (n <= 0) {
    return 0;
  }
  std::vector<StrEntry> arr(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> arr[i].str;
    arr[i].lcp = 0;
  }
  mergeSortLcpRec(arr, 0, n - 1);
  for (int i = 0; i < n; ++i) {
    std::cout << arr[i].str << "\n";
  }
  return 0;
}