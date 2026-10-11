#!/usr/bin/env bash
# End-to-end check of the CHEHAB CKKS path on the Quad benchmark:
#   DSL (benchmarks/quad_ckks) -> Compiler::compile -> Compiler::gen_he_code
#   -> generated SEAL CKKS code -> build -> encrypted run -> correctness check
#
#   tests/ckks/run_quad_ckks.sh [slot_count]
#
# Builds in <repo>/build-ckks with FHECO_BUILD_CKKS_BENCHMARKS=ON. SEAL is found
# through CMAKE_PREFIX_PATH (default $HOME/.local). Exits non-zero if any
# acceptance check fails.
set -euo pipefail

slot_count="${1:-8}"
repo="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
build="$repo/build-ckks"
prefix="${CMAKE_PREFIX_PATH:-$HOME/.local}"
bench="$build/benchmarks/quad_ckks"
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
cmake --build "$build" -j8 --target quad_ckks > /dev/null 2>&1

# Compile phase: CHEHAB compiles the DSL program and generates SEAL code.
cd "$bench"
python3 generate_quad_ckks.py --slot_count "$slot_count"
./quad_ckks "$slot_count" 1 1 1 > compile_output.txt
echo "---- CHEHAB compile phase"
grep -A1 "Compile time" compile_output.txt | tail -1 | sed 's/^/compile time: /'
grep -m1 "^max:" compile_output.txt | sed 's/^/depth (depth, xdepth) /'
echo "---- generated fhe()"
sed -n '/^{/,/^}/p' he/_gen_he_fhe.cpp

gen="$(cat he/_gen_he_fhe.cpp he/_gen_he_fhe.hpp he/main.cpp)"
has() { grep -qF -- "$1" <<< "$gen" && echo 1 || echo 0; }
expect "generated code uses scheme_type::ckks" "$(has 'scheme_type::ckks')"
expect "generated code uses CKKSEncoder" "$(has 'CKKSEncoder')"
expect "generated code multiplies ciphertexts" "$(has 'evaluator.multiply(')"
expect "generated code relinearizes" "$(has 'evaluator.relinearize(')"
expect "generated code calls rescale_to_next" "$(has 'evaluator.rescale_to_next(')"

# Execution phase: build and run the generated CKKS program.
rm -rf he/build
cmake -S he -B he/build -DCMAKE_PREFIX_PATH="$prefix" -DCMAKE_BUILD_TYPE=Release > /dev/null
cmake --build he/build -j8 > /dev/null 2>&1
echo "---- generated program run"
run_status=0
(cd he/build && ./main --tol 1e-6) > run_output.txt || run_status=$?
cat run_output.txt
expect "generated program exits 0 (decrypted y within 1e-6 of x^2)" "$([[ $run_status == 0 ]] && echo 1 || echo 0)"
expect "exactly one CKKS level consumed" "$(grep -q 'levels consumed 1)' run_output.txt && echo 1 || echo 0)"

if [[ $status == 0 ]]; then
  echo "Quad CKKS end-to-end: PASS"
else
  echo "Quad CKKS end-to-end: FAIL"
fi
exit $status
