#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "string_generator.h"
#include "string_sort_tester.h"

int main() {
  StringGenerator gen(42);
  StringSortTester tester;

  std::vector<int> sizes;
  for (int n = 100; n <= 3000; n += 100) {
    sizes.push_back(n);
  }
  std::vector<std::string> typeNames = {"Random", "Reverse", "AlmostSorted"};
  std::vector<std::vector<std::string>> baseArrays(3);
  baseArrays[0] = gen.generateRandom(3000);
  baseArrays[1] = gen.generateReverseSorted(3000);
  baseArrays[2] = gen.generateAlmostSorted(3000, 30);

  std::vector<std::string> sortNames = {
      "std_sort", "std_stable_sort", "string_quick", "merge_lcp", "msd_radix", "msd_radix_quick"};

  const int kIterations = 5;
  std::ofstream fout("result.csv");
  fout << "Size,Type,Sort,TimeMs,CharComparisons\n";

  for (int size : sizes) {
    for (int t = 0; t < 3; ++t) {
      for (int s = 0; s < 6; ++s) {
        double totalTime = 0.0;
        int64_t totalComps = 0;
        for (int it = 0; it < kIterations; ++it) {
          std::vector<std::string> arr(baseArrays[t].begin(), baseArrays[t].begin() + size);
          SortResult res;
          if (s == 0) {
            res = tester.testStdSort(arr);
          } else if (s == 1) {
            res = tester.testStdMergeSort(arr);
          } else if (s == 2) {
            res = tester.testStringQuickSort(arr);
          } else if (s == 3) {
            res = tester.testStringMergeSortLcp(arr);
          } else if (s == 4) {
            res = tester.testMsdRadixSort(arr);
          } else {
            res = tester.testMsdRadixQuickSort(arr);
          }
          totalTime += res.timeMs;
          totalComps += res.comparisons;
        }
        double avgTime = totalTime / kIterations;
        int64_t avgComps = totalComps / kIterations;
        fout << size << "," << typeNames[t] << "," << sortNames[s] << ","
             << avgTime << "," << avgComps << "\n";
      }
    }
    std::cout << "size " << size << " done\n";
  }
  fout.close();
  std::cout << "results written to result.csv\n";
  return 0;
}
