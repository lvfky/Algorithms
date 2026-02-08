#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "hash_func_gen.h"
class HyperLogLog {
 public:
  HyperLogLog(int b_bits, HashFuncGen hasher);
  void add(const std::string &s);
  double estimate() const;
  int B() const;
  uint32_t m() const;
 private:
  int b_;
  uint32_t m_;
  HashFuncGen hasher_;
  std::vector<uint8_t> reg_;
  static double alpha_(uint32_t m);
  uint8_t rho32_(uint32_t w) const;
};