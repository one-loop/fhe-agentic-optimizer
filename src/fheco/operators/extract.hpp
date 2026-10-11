#pragma once

#include "fheco/dsl/ciphertext.hpp"
#include "fheco/dsl/plaintext.hpp"

#include <cstddef>
#include <vector>

namespace fheco::operators
{
struct CipherTensor
{
  std::vector<Ciphertext> chunks;
  std::size_t logical_size;
};

Ciphertext extract(
  const Ciphertext &input, const Plaintext &range_mask, std::size_t input_size,
  std::size_t extracted_size, std::size_t offset);

Ciphertext extract_linear(
  const Ciphertext &input, const Plaintext &projection_diagonal, std::size_t input_size,
  std::size_t extracted_size, std::size_t offset);

CipherTensor extract_sparse(
  const CipherTensor &input, const Plaintext &keep_front_mask, const Plaintext &drop_front_mask,
  std::size_t num_dense, std::size_t vocab_size);
} // namespace fheco::operators
