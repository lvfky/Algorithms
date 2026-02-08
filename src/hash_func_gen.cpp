#include "hash_func_gen.h"
HashFuncGen::HashFuncGen(uint64_t seed) : seed_(seed) {}
uint64_t HashFuncGen::fnv1a64_(const std::string &s) {
  uint64_t h = 1469598103934665603ULL;
  for (unsigned char c : s) {
    h ^= static_cast<uint64_t>(c);
    h *= 1099511628211ULL;
  }
  return h;
}
uint64_t HashFuncGen::mix64_(uint64_t z) {
  z += 0x9E3779B97F4A7C15ULL;
  z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
  z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
  return z ^ (z >> 31);
}
uint64_t HashFuncGen::hash64(const std::string &s) const {
  uint64_t h = fnv1a64_(s);
  h ^= mix64_(seed_);
  return mix64_(h);
}
uint32_t HashFuncGen::hash32(const std::string &s) const {
  return static_cast<uint32_t>(hash64(s) & 0xFFFFFFFFu);
}