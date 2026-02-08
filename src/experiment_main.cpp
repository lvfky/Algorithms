#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include "hash_func_gen.h"
#include "hll.h"
#include "hll_plus.h"
#include "random_stream_gen.h"
using std::cout;
using std::string;
using std::vector;
static double mean(const vector<double> &v) {
  if (v.empty()) {
    return 0.0;
  }
  double s = 0.0;
  for (double x : v) {
    s += x;
  }
  return s / static_cast<double>(v.size());
}
static double stdev(const vector<double> &v) {
  if (v.size() < 2) {
    return 0.0;
  }
  double mu = mean(v);
  double acc = 0.0;
  for (double x : v) {
    double d = x - mu;
    acc += d * d;
  }
  return std::sqrt(acc / static_cast<double>(v.size() - 1));
}
int main(int argc, char **argv) {
  int b_bits = 14;
  int streams = 20;
  size_t stream_len = 200000;
  int steps = 20;
  if (argc >= 2) {
    b_bits = std::stoi(argv[1]);
  }
  if (argc >= 3) {
    streams = std::stoi(argv[2]);
  }
  if (argc >= 4) {
    stream_len = static_cast<size_t>(std::stoll(argv[3]));
  }
  if (argc >= 5) {
    steps = std::stoi(argv[4]);
  }
  std::ofstream raw("data/raw_runs.csv");
  raw << "variant,stream_id,step,processed,true_unique,estimate\n";
  vector<vector<double>> est_std(steps), tru_std(steps);
  vector<vector<double>> est_plus(steps), tru_plus(steps);
  for (int s = 0; s < streams; ++s) {
    RandomStreamGen gen(1234567ULL + static_cast<uint64_t>(s) * 99991ULL, stream_len);
    HashFuncGen hasher_std(0xBADC0FFEEULL + static_cast<uint64_t>(s) * 1337ULL);
    HashFuncGen hasher_plus(0xC0FFEEULL + static_cast<uint64_t>(s) * 7331ULL);
    HyperLogLog hll(b_bits, hasher_std);
    HyperLogLogPlus hllp(b_bits, hasher_plus);
    std::unordered_set<string> exact;
    exact.reserve(stream_len / 2);
    int step_id = 0;
    size_t next_cut = stream_len / static_cast<size_t>(steps);
    while (gen.hasNext()) {
      string x = gen.next();
      exact.insert(x);
      hll.add(x);
      hllp.add(x);
      if (gen.produced() == next_cut || !gen.hasNext()) {
        double tru = static_cast<double>(exact.size());
        double e1 = hll.estimate();
        double e2 = hllp.estimate();
        raw << "standard," << s << "," << (step_id + 1) << "," << gen.produced() << "," << tru
            << "," << e1 << "\n";
        raw << "improved," << s << "," << (step_id + 1) << "," << gen.produced() << "," << tru
            << "," << e2 << "\n";
        tru_std[step_id].push_back(tru);
        est_std[step_id].push_back(e1);
        tru_plus[step_id].push_back(tru);
        est_plus[step_id].push_back(e2);
        ++step_id;
        next_cut = (static_cast<size_t>(step_id) + 1) * stream_len / static_cast<size_t>(steps);
      }
    }
  }
  auto dump_summary = [&](const string &name, const vector<vector<double>> &tru,
                          const vector<vector<double>> &est) {
    std::ofstream out("data/summary_" + name + ".csv");
    out << "step,processed,mean_true,mean_est,std_est,mean_rel_err,std_rel_err\n";
    for (int t = 0; t < steps; ++t) {
      double mu_true = mean(tru[t]);
      double mu_est = mean(est[t]);
      double sd_est = stdev(est[t]);
      vector<double> rel;
      rel.reserve(est[t].size());
      for (size_t i = 0; i < est[t].size(); ++i) {
        double denom = (tru[t][i] <= 0.0) ? 1.0 : tru[t][i];
        rel.push_back((est[t][i] - tru[t][i]) / denom);
      }
      double mu_rel = mean(rel);
      double sd_rel = stdev(rel);
      size_t processed = (static_cast<size_t>(t) + 1) * stream_len / static_cast<size_t>(steps);
      out << (t + 1) << "," << processed << "," << mu_true << "," << mu_est << "," << sd_est << ","
          << mu_rel << "," << sd_rel << "\n";
    }
  };
  dump_summary("standard", tru_std, est_std);
  dump_summary("improved", tru_plus, est_plus);
  cout << "Saved: data/raw_runs.csv, data/summary_standard.csv, data/summary_improved.csv\n";
  cout << "B=" << b_bits << " m=" << (1u << b_bits) << "\n";
  return 0;
}