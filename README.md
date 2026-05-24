# SET 9 — String Sorts Research

Эмпирическое исследование адаптированных алгоритмов сортировки строк:
тернарный String Quicksort, LCP-Mergesort, MSD Radix Sort, MSD Radix +
String Quicksort. Сравнение со стандартными `std::sort` и `std::stable_sort`
по времени и числу посимвольных сравнений.

## Структура

- `src/` — исходники алгоритмов, бенчмарк
  - `src/cf/` — самостоятельные файлы для отправки в CodeForces (A1m/q/r/rq)
- `data/` — результаты замеров (`result.csv`)
- `plots/` — построенные графики (PNG)
- `report/` — итоговый отчёт (PDF)
- `scripts/` — Python для построения графиков

## Сборка и запуск

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

./build/bench                       # генерирует result.csv в текущей директории
mv result.csv data/                 # положить в нужное место

pip install pandas matplotlib seaborn
python scripts/plot_results.py      # перечитывает data/result.csv -> plots/*.png
```
