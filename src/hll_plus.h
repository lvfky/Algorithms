#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include "hash_func_gen.h"
class HyperLogLogPlus {
 public:
  HyperLogLogPlus(int b_bits, HashFuncGen hasher);
  void add(const std::string &s);
  double estimate() const;
  int B() const;
  uint32_t m() const;
  size_t memoryBytesApprox() const;
 private:
  int b_;
  uint32_t m_;
  HashFuncGen hasher_;
  bool dense_;
  std::vector<uint8_t> reg_dense_;
  std::unordered_map<uint32_t, uint8_t> reg_sparse_;
  void ensureDense_();
  uint8_t rho64_(uint64_t w) const;
  static double alpha_(uint32_t m);
};