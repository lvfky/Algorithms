#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include "hash_func_gen.h"
#include "random_stream_gen.h"
using std::cout;
using std::string;
using std::vector;
int main() {
  const size_t n = 300000;
  const int bins = 1024;
  RandomStreamGen gen(777, n);
  HashFuncGen hasher(0xDEADBEEF);
  vector<uint64_t> freq(bins, 0);
  while (gen.hasNext()) {
    string s = gen.next();
    uint32_t h = hasher.hash32(s);
    int b = static_cast<int>((h >> 22) & (bins - 1));
    freq[b] += 1;
  }
  double expected = static_cast<double>(n) / static_cast<double>(bins);
  double chi2 = 0.0;
  uint64_t mx = 0;
  uint64_t mn = std::numeric_limits<uint64_t>::max();
  for (uint64_t f : freq) {
    mx = std::max(mx, f);
    mn = std::min(mn, f);
    double d = static_cast<double>(f) - expected;
    chi2 += (d * d) / expected;
  }
  std::ofstream out("data/hash_bins.csv");
  out << "bin,count\n";
  for (int i = 0; i < bins; ++i) {
    out << i << "," << freq[i] << "\n";
  }
  cout << "Saved: data/hash_bins.csv\n";
  cout << "bins=" << bins << " n=" << n << " min=" << mn << " max=" << mx
       << " max/avg=" << (static_cast<double>(mx) / expected) << " chi2=" << chi2 << "\n";
  return 0;
}