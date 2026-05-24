import re

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns


def safe_name(s):
    return re.sub(r"[^a-zA-Z0-9_\-]", "_", str(s))


def plot_by_group(df, metric, ylabel, group_col, hue_col, title_prefix, file_prefix):
    for value in df[group_col].unique():
        subset = df[df[group_col] == value]
        plt.figure(figsize=(12, 6))
        sns.lineplot(
            data=subset,
            x="Size",
            y=metric,
            hue=hue_col,
            markers=True,
            dashes=False,
        )
        plt.title(f"{title_prefix}: {value}")
        plt.xlabel("Размер массива (n)")
        plt.ylabel(ylabel)
        plt.grid(True, alpha=0.3)
        plt.legend(title=hue_col, fontsize=8)
        plt.tight_layout()
        plt.savefig(f"{file_prefix}_{safe_name(value)}.png", dpi=120)
        plt.close()


def plot_overall(df, metric, ylabel, title, file_name):
    plt.figure(figsize=(14, 7))
    sns.lineplot(
        data=df,
        x="Size",
        y=metric,
        hue="Sort",
        style="Type",
        markers=True,
        dashes=False,
    )
    plt.title(title)
    plt.xlabel("Размер массива (n)")
    plt.ylabel(ylabel)
    plt.grid(True, alpha=0.3)
    plt.legend(fontsize=8, ncol=2)
    plt.tight_layout()
    plt.savefig(file_name, dpi=120)
    plt.close()


def main():
    df = pd.read_csv("result.csv")
    plot_overall(df, "TimeMs", "Время (мс)",
                 "Время сортировки в зависимости от размера", "plot_time.png")
    plot_overall(df, "CharComparisons", "Количество посимвольных сравнений",
                 "Число сравнений в зависимости от размера", "plot_comparisons.png")

    plot_by_group(df, "TimeMs", "Время (мс)", "Sort", "Type",
                  "Время сортировки по алгоритму", "plot_time_by_sort")
    plot_by_group(df, "CharComparisons", "Сравнения", "Sort", "Type",
                  "Число сравнений по алгоритму", "plot_comparisons_by_sort")

    plot_by_group(df, "TimeMs", "Время (мс)", "Type", "Sort",
                  "Время сортировки по типу данных", "plot_time_by_type")
    plot_by_group(df, "CharComparisons", "Сравнения", "Type", "Sort",
                  "Число сравнений по типу данных", "plot_comparisons_by_type")

    print("plots generated")


if __name__ == "__main__":
    main()
