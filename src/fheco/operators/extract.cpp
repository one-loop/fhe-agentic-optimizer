#include "fheco/operators/extract.hpp"

#include "fheco/dsl/compiler.hpp"
#include "fheco/dsl/ops_overloads.hpp"

#include <stdexcept>
#include <utility>

namespace fheco::operators
{
namespace
{
void validate_extract_range(std::size_t input_size, std::size_t extracted_size, std::size_t offset)
{
  const auto slot_count = Compiler::active_func()->slot_count();
  if (input_size == 0 || input_size > slot_count)
    throw std::invalid_argument("extract input_size must be in [1, slot_count]");
  if (extracted_size == 0 || offset > input_size || extracted_size > input_size - offset)
    throw std::invalid_argument("extract range is outside the logical input");
}
} // namespace

Ciphertext extract(
  const Ciphertext &input, const Plaintext &range_mask, std::size_t input_size,
  std::size_t extracted_size, std::size_t offset)
{
  validate_extract_range(input_size, extracted_size, offset);
  Ciphertext result = (input * range_mask) << static_cast<int>(offset);
  result.set_shape({extracted_size});
  return result;
}

Ciphertext extract_linear(
  const Ciphertext &input, const Plaintext &projection_diagonal, std::size_t input_size,
  std::size_t extracted_size, std::size_t offset)
{
  return extract(input, projection_diagonal, input_size, extracted_size, offset);
}

CipherTensor extract_sparse(
  const CipherTensor &input, const Plaintext &keep_front_mask, const Plaintext &drop_front_mask,
  std::size_t num_dense, std::size_t vocab_size)
{
  const auto slot_count = Compiler::active_func()->slot_count();
  if (input.chunks.empty())
    throw std::invalid_argument("extract_sparse requires at least one ciphertext chunk");
  if (num_dense == 0 || num_dense >= slot_count)
    throw std::invalid_argument("extract_sparse num_dense must be in [1, slot_count)");
  if (vocab_size == 0)
    throw std::invalid_argument("extract_sparse vocab_size must be positive");
  if (input.logical_size != num_dense + vocab_size)
    throw std::invalid_argument("extract_sparse logical size does not match dense and sparse sizes");
  if (input.chunks.size() != (input.logical_size + slot_count - 1) / slot_count)
    throw std::invalid_argument("extract_sparse chunk count does not match the logical input size");

  const auto output_chunk_count = (vocab_size + slot_count - 1) / slot_count;
  std::vector<Ciphertext> shifted_parts;
  shifted_parts.reserve(output_chunk_count);
  for (std::size_t i = 0; i < output_chunk_count; ++i)
    shifted_parts.push_back((input.chunks[i] * drop_front_mask) << static_cast<int>(num_dense));

  std::vector<Ciphertext> output;
  output.reserve(output_chunk_count);
  for (std::size_t i = 0; i < output_chunk_count; ++i)
  {
    if (i + 1 == input.chunks.size())
      output.push_back(shifted_parts[i]);
    else
      output.push_back(shifted_parts[i] + ((input.chunks[i + 1] * keep_front_mask) << static_cast<int>(num_dense)));
  }
  return CipherTensor{std::move(output), vocab_size};
}
} // namespace fheco::operators
