import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def _load_numeric(path):
    df = pd.read_csv(path)
    for c in ["processed","mean_true","mean_est","std_est","mean_rel_err","std_rel_err"]:
        if c in df.columns:
            df[c] = pd.to_numeric(df[c], errors="coerce")
    df = df.dropna()
    return df

def plot_compare(summary_csv, out_path, title):
    df = _load_numeric(summary_csv)
    plt.figure()
    plt.plot(df["processed"].to_numpy(dtype=float), df["mean_true"].to_numpy(dtype=float), label="F_t^0 (mean true)")
    plt.plot(df["processed"].to_numpy(dtype=float), df["mean_est"].to_numpy(dtype=float), label="N_t (mean estimate)")
    plt.xlabel("processed items")
    plt.ylabel("unique count")
    plt.title(title)
    plt.legend()
    plt.tight_layout()
    plt.savefig(out_path, dpi=200)
    plt.close()

def plot_stats(summary_csv, out_path, title):
    df = _load_numeric(summary_csv)
    x = df["processed"].to_numpy(dtype=float)
    y = df["mean_est"].to_numpy(dtype=float)
    s = df["std_est"].to_numpy(dtype=float)
    plt.figure()
    plt.plot(x, y, label="E(N_t)")
    plt.fill_between(x, y - s, y + s, alpha=0.25, label="E(N_t) ± σ")
    plt.xlabel("processed items")
    plt.ylabel("estimate")
    plt.title(title)
    plt.legend()
    plt.tight_layout()
    plt.savefig(out_path, dpi=200)
    plt.close()

def plot_relerr(summary_csv, out_path, title):
    df = _load_numeric(summary_csv)
    x = df["processed"].to_numpy(dtype=float)
    y = df["mean_rel_err"].to_numpy(dtype=float)
    s = df["std_rel_err"].to_numpy(dtype=float)
    plt.figure()
    plt.plot(x, y, label="mean relative error")
    plt.fill_between(x, y - s, y + s, alpha=0.25, label="± σ")
    plt.xlabel("processed items")
    plt.ylabel("(N_t - F_t^0) / F_t^0")
    plt.title(title)
    plt.legend()
    plt.tight_layout()
    plt.savefig(out_path, dpi=200)
    plt.close()

def plot_hash_bins(bins_csv, out_path):
    df = pd.read_csv(bins_csv)
    df["bin"] = pd.to_numeric(df["bin"], errors="coerce")
    df["count"] = pd.to_numeric(df["count"], errors="coerce")
    df = df.dropna()
    plt.figure(figsize=(10,3))
    plt.bar(df["bin"].to_numpy(dtype=float), df["count"].to_numpy(dtype=float))
    plt.xlabel("bin (top bits)")
    plt.ylabel("count")
    plt.title("Hash uniformity check (histogram by bins)")
    plt.tight_layout()
    plt.savefig(out_path, dpi=200)
    plt.close()

plot_compare("data/summary_standard.csv", "plots/plot_compare_standard.png", "Standard HLL: mean estimate vs mean true")
plot_stats("data/summary_standard.csv", "plots/plot_stats_standard.png", "Standard HLL: E(N_t) ± σ")
plot_relerr("data/summary_standard.csv", "plots/plot_relerr_standard.png", "Standard HLL: relative error")

plot_compare("data/summary_improved.csv", "plots/plot_compare_improved.png", "Improved HLL (64-bit + sparse): mean estimate vs mean true")
plot_stats("data/summary_improved.csv", "plots/plot_stats_improved.png", "Improved HLL: E(N_t) ± σ")
plot_relerr("data/summary_improved.csv", "plots/plot_relerr_improved.png", "Improved HLL: relative error")

plot_hash_bins("data/hash_bins.csv", "plots/hash_uniformity.png")
print("Saved plots to /plots")