#include "hll.h"
#include <cmath>
HyperLogLog::HyperLogLog(int b_bits, HashFuncGen hasher)
    : b_(b_bits), m_(1u << b_bits), hasher_(hasher), reg_(m_, 0) {}
int HyperLogLog::B() const {
  return b_;
}
uint32_t HyperLogLog::m() const {
  return m_;
}
double HyperLogLog::alpha_(uint32_t m) {
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
uint8_t HyperLogLog::rho32_(uint32_t w) const {
  if (w == 0) {
    return static_cast<uint8_t>((32 - b_) + 1);
  }
#if defined(__GNUG__) || defined(__clang__)
  int lz = __builtin_clz(w);
#else
  int lz = 0;
  while ((w & 0x80000000u) == 0u) {
    ++lz;
    w <<= 1;
  }
#endif
  return static_cast<uint8_t>(lz + 1);
}
void HyperLogLog::add(const std::string &s) {
  uint32_t x = hasher_.hash32(s);
  uint32_t idx = x >> (32 - b_);
  uint32_t w = (b_ == 32) ? 0u : (x << b_);
  uint8_t r = rho32_(w);
  if (r > reg_[idx]) {
    reg_[idx] = r;
  }
}
double HyperLogLog::estimate() const {
  double z = 0.0;
  int zeros = 0;
  for (uint8_t v : reg_) {
    z += std::ldexp(1.0, -static_cast<int>(v));
    if (v == 0) {
      ++zeros;
    }
  }
  double raw = alpha_(m_) * static_cast<double>(m_) * static_cast<double>(m_) / z;
  if (raw <= 2.5 * static_cast<double>(m_) && zeros > 0) {
    return static_cast<double>(m_) * std::log(static_cast<double>(m_) / static_cast<double>(zeros));
  }
  const double two32 = 4294967296.0;
  if (raw > (1.0 / 30.0) * two32) {
    return -two32 * std::log(1.0 - raw / two32);
  }
  return raw;
}