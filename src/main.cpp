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

  struct Dataset {
    std::string label;
    std::vector<std::string> data;
  };
  std::vector<Dataset> datasets = {
      {"Random",       gen.generateRandom(3000)},
      {"Reverse",      gen.generateReverseSorted(3000)},
      {"AlmostSorted", gen.generateAlmostSorted(3000, 30)},
  };

  using TestFn = SortResult (StringSortTester::*)(std::vector<std::string>);
  struct Algo {
    std::string label;
    TestFn fn;
  };
  std::vector<Algo> algos = {
      {"std_sort",        &StringSortTester::testStdSort},
      {"std_stable_sort", &StringSortTester::testStdMergeSort},
      {"string_quick",    &StringSortTester::testStringQuickSort},
      {"merge_lcp",       &StringSortTester::testStringMergeSortLcp},
      {"msd_radix",       &StringSortTester::testMsdRadixSort},
      {"msd_radix_quick", &StringSortTester::testMsdRadixQuickSort},
  };

  const int kIterations = 5;
  std::ofstream fout("result.csv");
  fout << "Size,Type,Sort,TimeMs,CharComparisons\n";

  for (int size : sizes) {
    for (const auto &ds : datasets) {
      for (const auto &algo : algos) {
        double totalTime = 0.0;
        int64_t totalComps = 0;
        for (int it = 0; it < kIterations; ++it) {
          std::vector<std::string> arr(ds.data.begin(), ds.data.begin() + size);
          SortResult res = (tester.*algo.fn)(arr);
          totalTime += res.timeMs;
          totalComps += res.comparisons;
        }
        fout << size << "," << ds.label << "," << algo.label << ","
             << (totalTime / kIterations) << "," << (totalComps / kIterations) << "\n";
      }
    }
    std::cout << "size " << size << " done\n";
  }
  std::cout << "results written to result.csv\n";
  return 0;
}
