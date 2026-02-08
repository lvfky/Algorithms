#include "random_stream_gen.h"
#include <string>
RandomStreamGen::RandomStreamGen(uint64_t seed, size_t total_items)
    : rng_(seed), total_(total_items), pos_(0) {}
void RandomStreamGen::reset(uint64_t seed) {
  rng_.seed(seed);
  pos_ = 0;
}
bool RandomStreamGen::hasNext() const {
  return pos_ < total_;
}
size_t RandomStreamGen::total() const {
  return total_;
}
size_t RandomStreamGen::produced() const {
  return pos_;
}
char RandomStreamGen::randChar_() {
  static const std::string alphabet =
      "abcdefghijklmnopqrstuvwxyz"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "0123456789-";
  std::uniform_int_distribution<int> pick(0, static_cast<int>(alphabet.size() - 1));
  return alphabet[pick(rng_)];
}
std::string RandomStreamGen::makeToken_() {
  std::uniform_int_distribution<int> len_pick(1, 30);
  int len = len_pick(rng_);
  std::string s;
  s.reserve(static_cast<size_t>(len));
  for (int i = 0; i < len; ++i) {
    s.push_back(randChar_());
  }
  return s;
}
std::string RandomStreamGen::next() {
  ++pos_;
  return makeToken_();
}