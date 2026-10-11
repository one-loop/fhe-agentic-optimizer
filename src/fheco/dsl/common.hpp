#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace fheco
{
using integer = std::int64_t;

using PackedVal = std::vector<integer>;

enum class SecurityLevel
{
  none,
  tc128,
  tq128,
  tc192, 
  tq192,
  tc256,
  tq256
};

enum class Scheme
{
  bfv,
  ckks
};

// CKKS encryption parameters. They are given explicitly when a CKKS function
// is created; there is no automatic CKKS parameter selection.
struct CkksParams
{
  std::size_t poly_modulus_degree;
  // Bit sizes of the coefficient modulus primes, special prime last.
  std::vector<int> coeff_mod_bit_sizes;
  // Fresh inputs are encoded at scale 2^log2_scale.
  int log2_scale;
};

void validate_shape(const std::vector<std::size_t> &shape);

} // namespace fheco
