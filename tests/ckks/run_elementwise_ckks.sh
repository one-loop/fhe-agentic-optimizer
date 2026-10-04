#!/usr/bin/env bash
# End-to-end check of the CHEHAB CKKS path on the elementwise add and multiply
# operators (ciphertext-ciphertext, ciphertext-plaintext, ciphertext-scalar):
#   DSL (benchmarks/elementwise_ckks) -> Compiler::compile -> Compiler::gen_he_code
#   -> generated SEAL CKKS code -> build -> encrypted run -> correctness check
#
#   tests/ckks/run_elementwise_ckks.sh [slot_count]
#
# Builds in $BUILD_DIR (default <repo>/build-ckks) with
# FHECO_BUILD_CKKS_BENCHMARKS=ON. SEAL is found through CMAKE_PREFIX_PATH
# (default $HOME/.local). Exits non-zero if any acceptance check fails.
set -euo pipefail

slot_count="${1:-8}"
repo="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
build="${BUILD_DIR:-$repo/build-ckks}"
prefix="${CMAKE_PREFIX_PATH:-$HOME/.local}"
bench="$build/benchmarks/elementwise_ckks"
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
cmake --build "$build" -j8 --target elementwise_ckks > /dev/null 2>&1

cd "$bench"
for op in add add_plain add_scalar mul mul_plain mul_scalar; do
  echo "======== op $op"
  # Compile phase: CHEHAB compiles the DSL program and generates SEAL code.
  python3 generate_elementwise_ckks.py --op "$op" --slot_count "$slot_count"
  ./elementwise_ckks "$op" "$slot_count" 1 1 1 > compile_output.txt
  echo "---- CHEHAB compile phase"
  grep -A1 "Compile time" compile_output.txt | tail -1 | sed 's/^/compile time: /'
  grep -m1 "^max:" compile_output.txt | sed 's/^/depth (depth, xdepth) /'
  echo "---- generated fhe()"
  body="$(sed -n '/^void fhe(/,/^}/p' he/_gen_he_fhe.cpp)"
  echo "$body"

  gen="$(cat he/_gen_he_fhe.cpp he/_gen_he_fhe.hpp he/main.cpp)"
  has() { grep -qF -- "$1" <<< "$gen" && echo 1 || echo 0; }
  lacks() { grep -qF -- "$1" <<< "$body" && echo 0 || echo 1; }
  expect "generated code uses scheme_type::ckks" "$(has 'scheme_type::ckks')"
  expect "generated code uses CKKSEncoder" "$(has 'CKKSEncoder')"
  case "$op" in
    add)
      expect "generated fhe() adds two ciphertexts" "$(has 'evaluator.add(')"
      expected_levels=0 ;;
    add_plain|add_scalar)
      expect "generated fhe() adds a plaintext" "$(has 'evaluator.add_plain(')"
      expected_levels=0 ;;
    mul)
      expect "generated fhe() multiplies two ciphertexts" "$(has 'evaluator.multiply(')"
      expect "generated fhe() relinearizes" "$(has 'evaluator.relinearize(')"
      expected_levels=1 ;;
    mul_plain|mul_scalar)
      expect "generated fhe() multiplies by a plaintext" "$(has 'evaluator.multiply_plain(')"
      expect "generated fhe() does not relinearize" "$(lacks 'evaluator.relinearize(')"
      expected_levels=1 ;;
  esac
  if [[ "$op" == add* ]]; then
    expect "generated fhe() does not multiply" "$(lacks 'evaluator.multiply')"
    expect "generated fhe() does not rescale" "$(lacks 'evaluator.rescale_to_next(')"
  else
    expect "generated fhe() calls rescale_to_next" "$(has 'evaluator.rescale_to_next(')"
  fi
  if [[ "$op" == *_plain || "$op" == *_scalar ]]; then
    expect "generated fhe() encodes the plaintext operand with ckks_encode" "$(has 'ckks_encode(encoder, ')"
  fi
  if [[ "$op" == *_scalar ]]; then
    expect "io file gives the scalar operand as one value" \
      "$([[ $(grep '^s ' fhe_io_example.txt | wc -w) -eq 4 ]] && echo 1 || echo 0)"
  fi

  # Execution phase: build and run the generated CKKS program.
  rm -rf he/build
  cmake -S he -B he/build -DCMAKE_PREFIX_PATH="$prefix" -DCMAKE_BUILD_TYPE=Release > /dev/null
  cmake --build he/build -j8 > /dev/null 2>&1
  echo "---- generated program run"
  run_status=0
  (cd he/build && ./main --tol 1e-6) > run_output.txt || run_status=$?
  cat run_output.txt
  expect "generated program exits 0 (decrypted y within 1e-6 of the numpy reference)" \
    "$([[ $run_status == 0 ]] && echo 1 || echo 0)"
  expect "exactly $expected_levels CKKS level(s) consumed" \
    "$(grep -q "levels consumed $expected_levels)" run_output.txt && echo 1 || echo 0)"
done

if [[ $status == 0 ]]; then
  echo "Elementwise add/mul CKKS end-to-end (6 variants): PASS"
else
  echo "Elementwise add/mul CKKS end-to-end (6 variants): FAIL"
fi
exit $status
