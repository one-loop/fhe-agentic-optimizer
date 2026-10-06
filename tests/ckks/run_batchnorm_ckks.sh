#!/usr/bin/env bash
# End-to-end check of BatchNorm1d/BatchNorm2d compiled through CHEHAB (CKKS):
#   DSL y = x * A + B (benchmarks/batchnorm_ckks) -> Compiler::compile
#   -> Compiler::gen_he_code -> generated SEAL CKKS code -> build -> encrypted
#   run -> check against the original eval-mode BatchNorm formula
#
#   tests/ckks/run_batchnorm_ckks.sh [case ...]   (default: bn1d bn2d bn1d_rand bn2d_rand)
#
# Builds in <repo>/build-ckks with FHECO_BUILD_CKKS_BENCHMARKS=ON. SEAL is found
# through CMAKE_PREFIX_PATH (default $HOME/.local). Exits non-zero if any
# acceptance check fails.
set -euo pipefail

cases=("$@")
if [[ ${#cases[@]} -eq 0 ]]; then
  cases=(bn1d bn2d bn1d_rand bn2d_rand)
fi
repo="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
build="$repo/build-ckks"
prefix="${CMAKE_PREFIX_PATH:-$HOME/.local}"
bench="$build/benchmarks/batchnorm_ckks"
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
cmake --build "$build" -j8 --target batchnorm_ckks generate_batchnorm_ckks > /dev/null 2>&1

cd "$bench"
for case_name in "${cases[@]}"; do
  echo "==== $case_name"
  slot_count="$(./generate_batchnorm_ckks "$case_name" | tee generate_output.txt | head -1)"
  tail -n +2 generate_output.txt

  # Compile phase: CHEHAB compiles the DSL program and generates SEAL code.
  ./batchnorm_ckks "$slot_count" 1 1 1 > compile_output.txt
  grep -A1 "Compile time" compile_output.txt | tail -1 | sed 's/^/compile time: /'
  grep -m1 "^max:" compile_output.txt | sed 's/^/CHEHAB (depth, xdepth) /'

  gen="$(cat he/_gen_he_fhe.cpp he/_gen_he_fhe.hpp he/main.cpp)"
  has() { grep -qF -- "$1" <<< "$gen" && echo 1 || echo 0; }
  expect "generated code uses scheme_type::ckks" "$(has 'scheme_type::ckks')"
  expect "generated code uses CKKSEncoder" "$(has 'CKKSEncoder')"
  expect "generated code encodes plaintexts at the ciphertext level" "$(has '.parms_id(), ')"
  expect "generated code calls multiply_plain" "$(has 'evaluator.multiply_plain(')"
  expect "generated code calls rescale_to_next" "$(has 'evaluator.rescale_to_next(')"
  expect "generated code calls add_plain" "$(has 'evaluator.add_plain(')"

  # Execution phase: build and run the generated CKKS program.
  rm -rf he/build
  cmake -S he -B he/build -DCMAKE_PREFIX_PATH="$prefix" -DCMAKE_BUILD_TYPE=Release > /dev/null
  cmake --build he/build -j8 > /dev/null 2>&1
  run_status=0
  (cd he/build && ./main --tol 1e-6) > run_output.txt || run_status=$?
  cat run_output.txt
  expect "generated program exits 0 (y within 1e-6 of the original BatchNorm formula)" \
    "$([[ $run_status == 0 ]] && echo 1 || echo 0)"
  expect "exactly one CKKS level consumed" "$(grep -q 'levels consumed 1)' run_output.txt && echo 1 || echo 0)"
done

echo "---- generated fhe() (last case)"
sed -n '/^{/,/^}/p' he/_gen_he_fhe.cpp

if [[ $status == 0 ]]; then
  echo "BatchNorm CKKS end-to-end: PASS"
else
  echo "BatchNorm CKKS end-to-end: FAIL"
fi
exit $status
