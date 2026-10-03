#!/usr/bin/env bash
# BFV regression check: the code CHEHAB generates for BFV programs must stay
# byte-identical while CKKS support is added.
#
#   tests/bfv_golden/run_bfv_golden.sh           compare against expected/
#   tests/bfv_golden/run_bfv_golden.sh --update  regenerate expected/
#
# Checks:
#   poly_reg, nn_linear, nn_conv2d, nn_avgpool2d (scalar path:
#             ./<bench> 0 4 0 0 1 1 1): generated he/ sources, he/main.cpp and
#             compiler stdout (minus the compile-time line); then builds and runs
#             each generated BFV program.
#   quad_bfv  (tests/bfv_golden/quad_bfv_probe.cpp, y = x * x) and
#   bn_bfv    (tests/bfv_golden/bn_bfv_probe.cpp, y = x * A + B with plaintext
#             inputs): generated sources and Quantifier output.
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
# Benchmarks compiled on the scalar path with slot count 4.
benchmarks=(poly_reg nn_linear nn_conv2d nn_avgpool2d)
targets=(fheco)
for bench in "${benchmarks[@]}"; do
  targets+=("$bench")
  for i in 2 3 4 5 6; do
    targets+=("copy_my_file_${i}${bench}")
  done
done
cmake --build "$build" -j8 --target "${targets[@]}" > /dev/null

for bench in "${benchmarks[@]}"; do
  dir="$build/benchmarks/$bench"
  (
    cd "$dir"
    python3 "generate_${bench}.py" --slot_count 4 > /dev/null
    ./"$bench" 0 4 0 0 1 1 1
  ) | strip_timing > "$work/${bench}_stdout.txt"
  check "$bench/stdout.txt" "$work/${bench}_stdout.txt"
  check "$bench/_gen_he_fhe.hpp" "$dir/he/_gen_he_fhe.hpp"
  check "$bench/_gen_he_fhe.cpp" "$dir/he/_gen_he_fhe.cpp"
  check "$bench/main.cpp" "$dir/he/main.cpp"

  rm -rf "$dir/he/build"
  cmake -S "$dir/he" -B "$dir/he/build" -DCMAKE_PREFIX_PATH="$prefix" > /dev/null
  cmake --build "$dir/he/build" -j8 > /dev/null 2>&1
  if (cd "$dir/he/build" && ./main > "$work/${bench}_run.txt" 2>&1); then
    echo "ran      $bench generated BFV program ($(grep -m1 'execution_time' "$work/${bench}_run.txt"))"
  else
    echo "FAILED   $bench generated BFV program"
    cat "$work/${bench}_run.txt"
    status=1
  fi
done

# --- DSL probes ---------------------------------------------------------
# <name>_probe.cpp: quad_bfv (y = x * x), bn_bfv (y = x * A + B, plaintext inputs)
for probe in quad_bfv bn_bfv; do
  mkdir -p "$work/$probe/he"
  c++ -std=c++17 -O1 -I"$repo/src" "$here/${probe}_probe.cpp" "$build/libfheco.a" -o "$work/$probe/probe"
  (cd "$work/$probe" && ./probe) > "$work/${probe}_stdout.txt"
  check "$probe/stdout.txt" "$work/${probe}_stdout.txt"
  check "$probe/_gen_he_fhe.hpp" "$work/$probe/he/_gen_he_fhe.hpp"
  check "$probe/_gen_he_fhe.cpp" "$work/$probe/he/_gen_he_fhe.cpp"
  check "$probe/main.cpp" "$work/$probe/he/main.cpp"
done

if [[ $update -eq 0 ]]; then
  if [[ $status -eq 0 ]]; then
    echo "BFV golden check: PASS"
  else
    echo "BFV golden check: FAIL"
  fi
fi
exit $status
