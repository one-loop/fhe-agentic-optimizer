#!/usr/bin/env bash
# End-to-end check of the degree-5 Chebyshev-Sigmoid compiled through CHEHAB
# (CKKS), validated in stages:
#   z = alpha*x + beta, T2, T3, then the full polynomial P,
# on the direct-SEAL reference's nine points, and P on 256 random points.
#
#   tests/ckks/run_chebyshev_ckks.sh
#
# FHE numerical error (decrypted vs the same plaintext computation) must be
# below 1e-6; polynomial approximation error (plaintext polynomial vs the true
# Sigmoid) is reported separately. Builds in <repo>/build-ckks with
# FHECO_BUILD_CKKS_BENCHMARKS=ON. SEAL is found through CMAKE_PREFIX_PATH
# (default $HOME/.local). Exits non-zero if any check fails.
set -euo pipefail

repo="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
build="$repo/build-ckks"
prefix="${CMAKE_PREFIX_PATH:-$HOME/.local}"
bench="$build/benchmarks/chebyshev_ckks"
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
cmake --build "$build" -j8 --target chebyshev_ckks generate_chebyshev_ckks > /dev/null 2>&1

cd "$bench"
# target points expected_levels
runs=("z points9 1" "t2 points9 2" "t3 points9 3" "p points9 6" "p dense 6")
for run in "${runs[@]}"; do
  read -r target points levels <<< "$run"
  echo "==== target $target, $points"
  slot_count="$(./generate_chebyshev_ckks "$target" "$points" | tee generate_output.txt | head -1)"
  tail -n +2 generate_output.txt

  ./chebyshev_ckks "$target" "$slot_count" 1 1 1 > compile_output.txt
  grep -A1 "Compile time" compile_output.txt | tail -1 | sed 's/^/compile time: /'
  grep -m1 "^max:" compile_output.txt | sed 's/^/CHEHAB (depth, xdepth) /'

  gen="$(cat he/_gen_he_fhe.cpp he/_gen_he_fhe.hpp he/main.cpp)"
  has() { grep -qF -- "$1" <<< "$gen" && echo 1 || echo 0; }
  expect "generated code uses scheme_type::ckks and CKKSEncoder" \
    "$([[ $(has 'scheme_type::ckks') == 1 && $(has 'CKKSEncoder') == 1 ]] && echo 1 || echo 0)"
  if [[ "$target" != z ]]; then
    expect "ciphertext multiply is relinearized and rescaled" \
      "$([[ $(has 'evaluator.multiply(') == 1 && $(has 'evaluator.relinearize(') == 1 && $(has 'evaluator.rescale_to_next(') == 1 ]] && echo 1 || echo 0)"
  fi
  if [[ "$target" == t3 || "$target" == p ]]; then
    expect "level mismatches are aligned with mod_switch_to_next" "$(has 'evaluator.mod_switch_to_next(')"
    expect "scale mismatches are aligned with match_scale (ckks_landing_scale)" "$(has 'ckks_landing_scale(context')"
  fi

  rm -rf he/build
  cmake -S he -B he/build -DCMAKE_PREFIX_PATH="$prefix" -DCMAKE_BUILD_TYPE=Release > /dev/null
  cmake --build he/build -j8 > /dev/null 2>&1
  run_status=0
  (cd he/build && ./main --tol 1e-6) > run_output.txt || run_status=$?
  cat run_output.txt
  expect "FHE numerical error below 1e-6" "$([[ $run_status == 0 ]] && echo 1 || echo 0)"
  expect "$levels CKKS levels consumed" "$(grep -q "levels consumed $levels)" run_output.txt && echo 1 || echo 0)"
done

echo "---- generated fhe() (target p)"
sed -n '/^void fhe/,/^}/p' he/_gen_he_fhe.cpp

if [[ $status == 0 ]]; then
  echo "Chebyshev CKKS end-to-end: PASS"
else
  echo "Chebyshev CKKS end-to-end: FAIL"
fi
exit $status
