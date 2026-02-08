#pragma once
#include <cstddef>
#include <cstdint>
#include <random>
#include <string>
class RandomStreamGen {
 public:
  RandomStreamGen(uint64_t seed, size_t total_items);
  void reset(uint64_t seed);
  bool hasNext() const;
  std::string next();
  size_t total() const;
  size_t produced() const;
 private:
  std::mt19937_64 rng_;
  size_t total_;
  size_t pos_;
  char randChar_();
  std::string makeToken_();
};