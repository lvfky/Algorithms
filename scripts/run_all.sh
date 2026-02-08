#!/usr/bin/env bash
set -euo pipefail
mkdir -p build
cmake -S . -B build
cmake --build build -j
./build/hash_uniformity
./build/experiment 14 20 200000 20
python3 scripts/plot_all.py
echo "Done."