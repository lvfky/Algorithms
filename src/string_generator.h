#pragma once

#include <algorithm>
#include <cstdint>
#include <random>
#include <string>
#include <vector>

class StringGenerator {
 public:
  static const std::string kAlphabet;

  StringGenerator() : rng_(std::random_device{}()) {}
  explicit StringGenerator(uint32_t seed) : rng_(seed) {}

  std::vector<std::string> generateRandom(int n, int minLen = 10, int maxLen = 200) {
    std::vector<std::string> arr(n);
    std::uniform_int_distribution<int> lenDist(minLen, maxLen);
    std::uniform_int_distribution<int> charDist(0, static_cast<int>(kAlphabet.size()) - 1);
    for (int i = 0; i < n; ++i) {
      int len = lenDist(rng_);
      std::string s;
      s.reserve(len);
      for (int j = 0; j < len; ++j) {
        s.push_back(kAlphabet[charDist(rng_)]);
      }
      arr[i] = std::move(s);
    }
    return arr;
  }

  std::vector<std::string> generateReverseSorted(int n, int minLen = 10, int maxLen = 200) {
    std::vector<std::string> arr = generateRandom(n, minLen, maxLen);
    std::sort(arr.begin(), arr.end(), std::greater<std::string>());
    return arr;
  }

  std::vector<std::string> generateAlmostSorted(int n, int swaps = 30, int minLen = 10, int maxLen = 200) {
    std::vector<std::string> arr = generateRandom(n, minLen, maxLen);
    std::sort(arr.begin(), arr.end());
    if (n <= 1) {
      return arr;
    }
    std::uniform_int_distribution<int> idxDist(0, n - 1);
    for (int i = 0; i < swaps; ++i) {
      int a = idxDist(rng_);
      int b = idxDist(rng_);
      if (a != b) {
        std::swap(arr[a], arr[b]);
      }
    }
    return arr;
  }

  std::vector<std::string> generateWithCommonPrefix(int n, int prefixLen, int minLen = 10, int maxLen = 200) {
    std::vector<std::string> arr(n);
    std::uniform_int_distribution<int> lenDist(std::max(minLen, prefixLen), maxLen);
    std::uniform_int_distribution<int> charDist(0, static_cast<int>(kAlphabet.size()) - 1);
    std::string prefix;
    prefix.reserve(prefixLen);
    for (int i = 0; i < prefixLen; ++i) {
      prefix.push_back(kAlphabet[charDist(rng_)]);
    }
    for (int i = 0; i < n; ++i) {
      int len = lenDist(rng_);
      std::string s = prefix;
      s.reserve(len);
      for (int j = prefixLen; j < len; ++j) {
        s.push_back(kAlphabet[charDist(rng_)]);
      }
      arr[i] = std::move(s);
    }
    return arr;
  }

 private:
  std::mt19937 rng_;
};

inline const std::string StringGenerator::kAlphabet =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#%:;^&*()-.";
