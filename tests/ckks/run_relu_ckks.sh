#!/usr/bin/env bash
# End-to-end check of the CHEHAB CKKS path on ReLU through a composite minimax
# sign approximation (benchmarks/relu_ckks, three degree-7 stages):
#   DSL -> Compiler::compile -> Compiler::gen_he_code -> generated SEAL CKKS
#   code -> build -> encrypted run -> correctness check
#
#   tests/ckks/run_relu_ckks.sh
#
# Builds in $BUILD_DIR (default <repo>/build-ckks) with
# FHECO_BUILD_CKKS_BENCHMARKS=ON. SEAL is found through CMAKE_PREFIX_PATH
# (default $HOME/.local). Exits non-zero if any acceptance check fails.
set -euo pipefail

repo="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
build="${BUILD_DIR:-$repo/build-ckks}"
prefix="${CMAKE_PREFIX_PATH:-$HOME/.local}"
bench="$build/benchmarks/relu_ckks"
status=0

expect() {
  local what="$1" ok="$2"
  if [[ "$ok" == 1 ]]; then
    echo "ok       $what"
  else
    echo "FAILED   $what"
    status=1
  fi
}

cmake -S "$repo" -B "$build" -DCMAKE_PREFIX_PATH="$prefix" -DCMAKE_BUILD_TYPE=Release \
  -DFHECO_BUILD_CKKS_BENCHMARKS=ON > /dev/null
cmake --build "$build" -j8 --target relu_ckks > /dev/null 2>&1

cd "$bench"
for points in points9 dense; do
  echo "======== $points"
  python3 generate_relu_ckks.py "$points" > generator_output.txt
  tail -n +2 generator_output.txt
  slot_count="$(head -1 generator_output.txt)"
  expected_levels="$(sed -n 's/^reference levels: //p' generator_output.txt)"

  # Compile phase: CHEHAB compiles the DSL program and generates SEAL code.
  ./relu_ckks "$slot_count" 1 1 1 > compile_output.txt
  echo "---- CHEHAB compile phase"
  grep -A1 "Compile time" compile_output.txt | tail -1 | sed 's/^/compile time: /'
  grep -m1 "^max:" compile_output.txt | sed 's/^/depth (depth, xdepth) /'
  echo "---- evaluator calls in the generated fhe()"
  sed -n '/^void fhe(/,/^}/p' he/_gen_he_fhe.cpp | grep -o 'evaluator\.[a-z_]*' | sort | uniq -c

  gen="$(cat he/_gen_he_fhe.cpp he/_gen_he_fhe.hpp he/main.cpp)"
  has() { grep -qF -- "$1" <<< "$gen" && echo 1 || echo 0; }
  expect "generated code uses scheme_type::ckks" "$(has 'scheme_type::ckks')"
  expect "generated code multiplies ciphertexts and relinearizes" \
    "$([[ $(has 'evaluator.multiply(') == 1 && $(has 'evaluator.relinearize(') == 1 ]] && echo 1 || echo 0)"
  expect "generated code encodes the scalar coefficients with ckks_encode" "$(has 'ckks_encode(encoder, ')"

  # Execution phase: build and run the generated CKKS program.
  rm -rf he/build
  cmake -S he -B he/build -DCMAKE_PREFIX_PATH="$prefix" -DCMAKE_BUILD_TYPE=Release > /dev/null
  cmake --build he/build -j8 > /dev/null 2>&1
  echo "---- generated program run"
  run_status=0
  (cd he/build && ./main --tol 1e-6) > run_output.txt || run_status=$?
  cat run_output.txt
  expect "generated program exits 0 (decrypted y within 1e-6 of the plaintext composite polynomial)" \
    "$([[ $run_status == 0 ]] && echo 1 || echo 0)"
  expect "exactly $expected_levels CKKS levels consumed" \
    "$(grep -q "levels consumed $expected_levels)" run_output.txt && echo 1 || echo 0)"
done

if [[ $status == 0 ]]; then
  echo "ReLU (composite minimax sign) CKKS end-to-end: PASS"
else
  echo "ReLU (composite minimax sign) CKKS end-to-end: FAIL"
fi
exit $status
