#pragma once
#include <cstdint>
#include <string>
class HashFuncGen {
 public:
  explicit HashFuncGen(uint64_t seed);
  uint32_t hash32(const std::string &s) const;
  uint64_t hash64(const std::string &s) const;
 private:
  uint64_t seed_;
  static uint64_t fnv1a64_(const std::string &s);
  static uint64_t mix64_(uint64_t z);
};