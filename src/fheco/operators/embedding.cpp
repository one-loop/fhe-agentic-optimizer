#include "fheco/operators/embedding.hpp"

#include "fheco/dsl/compiler.hpp"
#include "fheco/dsl/ops_overloads.hpp"

#include <stdexcept>

namespace fheco::operators
{
std::vector<Ciphertext> table_mult(
  const Ciphertext &indicator, const std::vector<Plaintext> &table_columns)
{
  if (table_columns.empty())
    throw std::invalid_argument("table_mult requires at least one table column");
  const auto slot_count = Compiler::active_func()->slot_count();
  if (slot_count == 0 || (slot_count & (slot_count - 1)) != 0)
    throw std::invalid_argument("table_mult requires a power-of-two packed slot count");

  std::vector<Ciphertext> output;
  output.reserve(table_columns.size());
  for (const auto &column : table_columns)
    output.push_back(reduce_add(indicator * column));
  return output;
}

Ciphertext table_mult_consolidate(
  const Ciphertext &indicator, const std::vector<Plaintext> &table_columns, const Plaintext &first_slot_mask)
{
  if (table_columns.size() > Compiler::active_func()->slot_count())
    throw std::invalid_argument("embedding dimension exceeds the packed slot count");

  auto coordinates = table_mult(indicator, table_columns);
  Ciphertext output = coordinates[0] * first_slot_mask;
  for (std::size_t i = 1; i < coordinates.size(); ++i)
    output += (coordinates[i] * first_slot_mask) >> static_cast<int>(i);
  output.set_shape({coordinates.size()});
  return output;
}
} // namespace fheco::operators
