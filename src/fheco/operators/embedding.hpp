#pragma once

#include "fheco/dsl/ciphertext.hpp"
#include "fheco/dsl/plaintext.hpp"

#include <vector>

namespace fheco::operators
{
std::vector<Ciphertext> table_mult(
  const Ciphertext &indicator, const std::vector<Plaintext> &table_columns);

Ciphertext table_mult_consolidate(
  const Ciphertext &indicator, const std::vector<Plaintext> &table_columns, const Plaintext &first_slot_mask);

inline Ciphertext embedding(
  const Ciphertext &one_hot, const std::vector<Plaintext> &table_columns, const Plaintext &first_slot_mask)
{
  return table_mult_consolidate(one_hot, table_columns, first_slot_mask);
}
} // namespace fheco::operators
