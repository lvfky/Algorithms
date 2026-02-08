#include "hll_plus.h"
#include <cmath>
HyperLogLogPlus::HyperLogLogPlus(int b_bits, HashFuncGen hasher)
    : b_(b_bits),
      m_(1u << b_bits),
      hasher_(hasher),
      dense_(false),
      reg_dense_(),
      reg_sparse_() {
  reg_sparse_.reserve(m_ / 8);
}
int HyperLogLogPlus::B() const {
  return b_;
}
uint32_t HyperLogLogPlus::m() const {
  return m_;
}
double HyperLogLogPlus::alpha_(uint32_t m) {
  if (m == 16) {
    return 0.673;
  }
  if (m == 32) {
    return 0.697;
  }
  if (m == 64) {
    return 0.709;
  }
  return 0.7213 / (1.0 + 1.079 / static_cast<double>(m));
}
uint8_t HyperLogLogPlus::rho64_(uint64_t w) const {
  if (w == 0ULL) {
    return static_cast<uint8_t>((64 - b_) + 1);
  }
#if defined(__GNUG__) || defined(__clang__)
  int lz = __builtin_clzll(w);
#else
  int lz = 0;
  while ((w & 0x8000000000000000ULL) == 0ULL) {
    ++lz;
    w <<= 1;
  }
#endif
  return static_cast<uint8_t>(lz + 1);
}
void HyperLogLogPlus::ensureDense_() {
  if (dense_) {
    return;
  }
  reg_dense_.assign(m_, 0);
  for (const auto &kv : reg_sparse_) {
    reg_dense_[kv.first] = kv.second;
  }
  reg_sparse_.clear();
  reg_sparse_.rehash(0);
  dense_ = true;
}
void HyperLogLogPlus::add(const std::string &s) {
  uint64_t x = hasher_.hash64(s);
  uint32_t idx = static_cast<uint32_t>(x >> (64 - b_));
  uint64_t w = (b_ == 64) ? 0ULL : (x << b_);
  uint8_t r = rho64_(w);
  if (!dense_) {
    auto it = reg_sparse_.find(idx);
    if (it == reg_sparse_.end()) {
      if (r > 0) {
        reg_sparse_.emplace(idx, r);
      }
    } else if (r > it->second) {
      it->second = r;
    }
    if (reg_sparse_.size() > m_ / 4) {
      ensureDense_();
    }
    return;
  }
  if (r > reg_dense_[idx]) {
    reg_dense_[idx] = r;
  }
}
double HyperLogLogPlus::estimate() const {
  double z = 0.0;
  int zeros = 0;
  if (!dense_) {
    int nonzero = static_cast<int>(reg_sparse_.size());
    zeros = static_cast<int>(m_) - nonzero;
    for (uint32_t i = 0; i < m_; ++i) {
      auto it = reg_sparse_.find(i);
      uint8_t v = (it == reg_sparse_.end()) ? 0 : it->second;
      z += std::ldexp(1.0, -static_cast<int>(v));
    }
  } else {
    for (uint8_t v : reg_dense_) {
      z += std::ldexp(1.0, -static_cast<int>(v));
      if (v == 0) {
        ++zeros;
      }
    }
  }
  double raw = alpha_(m_) * static_cast<double>(m_) * static_cast<double>(m_) / z;
  if (raw <= 2.5 * static_cast<double>(m_) && zeros > 0) {
    return static_cast<double>(m_) * std::log(static_cast<double>(m_) / static_cast<double>(zeros));
  }
  const long double two64 = 18446744073709551616.0L;
  if (raw > static_cast<double>((1.0L / 30.0L) * two64)) {
    long double r = static_cast<long double>(raw);
    return static_cast<double>(-two64 * std::log(1.0L - r / two64));
  }
  return raw;
}
size_t HyperLogLogPlus::memoryBytesApprox() const {
  if (dense_) {
    return reg_dense_.size() * sizeof(uint8_t);
  }
  return reg_sparse_.size() * (sizeof(uint32_t) + sizeof(uint8_t));
}