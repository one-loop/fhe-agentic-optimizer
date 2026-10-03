#!/usr/bin/env bash
# BFV regression check: the code CHEHAB generates for BFV programs must stay
# byte-identical while CKKS support is added.
#
#   tests/bfv_golden/run_bfv_golden.sh           compare against expected/
#   tests/bfv_golden/run_bfv_golden.sh --update  regenerate expected/
#
# Checks:
#   poly_reg  (scalar path: ./poly_reg 0 4 0 0 1 1 1): generated he/ sources,
#             he/main.cpp and compiler stdout (minus the compile-time line);
#             then builds and runs the generated BFV program.
#   quad_bfv  (tests/bfv_golden/quad_bfv_probe.cpp, y = x * x): generated
#             sources and Quantifier output.
#
# Uses the standard CHEHAB build directory <repo>/build (the benchmark CMake
# files hard-code it). SEAL is found through CMAKE_PREFIX_PATH (default
# $HOME/.local).
set -euo pipefail

update=0
if [[ "${1:-}" == "--update" ]]; then
  update=1
fi

repo="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
here="$repo/tests/bfv_golden"
expected="$here/expected"
build="$repo/build"
prefix="${CMAKE_PREFIX_PATH:-$HOME/.local}"
work="$(mktemp -d)"
trap 'rm -rf "$work"' EXIT

status=0

# Compare (or, with --update, store) one produced file.
check() {
  local name="$1" produced="$2"
  local want="$expected/$name"
  if [[ $update -eq 1 ]]; then
    mkdir -p "$(dirname "$want")"
    cp "$produced" "$want"
    echo "updated  $name"
  elif diff -u "$want" "$produced" > "$work/diff.txt"; then
    echo "same     $name"
  else
    echo "CHANGED  $name"
    cat "$work/diff.txt"
    status=1
  fi
}

# Drop lines that legitimately differ run to run (compile time).
strip_timing() {
  grep -vE '^[0-9.e+-]+ ms$'
}

cmake -S "$repo" -B "$build" -DCMAKE_PREFIX_PATH="$prefix" -DCMAKE_BUILD_TYPE=Release > /dev/null
cmake --build "$build" -j8 --target fheco poly_reg \
  copy_my_file_2poly_reg copy_my_file_3poly_reg copy_my_file_4poly_reg \
  copy_my_file_5poly_reg copy_my_file_6poly_reg > /dev/null

# --- poly_reg -----------------------------------------------------------
pr="$build/benchmarks/poly_reg"
(
  cd "$pr"
  python3 generate_poly_reg.py --slot_count 4
  ./poly_reg 0 4 0 0 1 1 1
) | strip_timing > "$work/poly_reg_stdout.txt"
check poly_reg/stdout.txt "$work/poly_reg_stdout.txt"
check poly_reg/_gen_he_fhe.hpp "$pr/he/_gen_he_fhe.hpp"
check poly_reg/_gen_he_fhe.cpp "$pr/he/_gen_he_fhe.cpp"
check poly_reg/main.cpp "$pr/he/main.cpp"

rm -rf "$pr/he/build"
cmake -S "$pr/he" -B "$pr/he/build" -DCMAKE_PREFIX_PATH="$prefix" > /dev/null
cmake --build "$pr/he/build" -j8 > /dev/null 2>&1
if (cd "$pr/he/build" && ./main > "$work/poly_reg_run.txt" 2>&1); then
  echo "ran      poly_reg generated BFV program ($(grep -m1 'execution_time' "$work/poly_reg_run.txt"))"
else
  echo "FAILED   poly_reg generated BFV program"
  cat "$work/poly_reg_run.txt"
  status=1
fi

# --- quad_bfv probe -----------------------------------------------------
mkdir -p "$work/quad_bfv/he"
c++ -std=c++17 -O1 -I"$repo/src" "$here/quad_bfv_probe.cpp" "$build/libfheco.a" -o "$work/quad_bfv/probe"
(cd "$work/quad_bfv" && ./probe) > "$work/quad_bfv_stdout.txt"
check quad_bfv/stdout.txt "$work/quad_bfv_stdout.txt"
check quad_bfv/_gen_he_fhe.hpp "$work/quad_bfv/he/_gen_he_fhe.hpp"
check quad_bfv/_gen_he_fhe.cpp "$work/quad_bfv/he/_gen_he_fhe.cpp"
check quad_bfv/main.cpp "$work/quad_bfv/he/main.cpp"

if [[ $update -eq 0 ]]; then
  if [[ $status -eq 0 ]]; then
    echo "BFV golden check: PASS"
  else
    echo "BFV golden check: FAIL"
  fi
fi
exit $status
