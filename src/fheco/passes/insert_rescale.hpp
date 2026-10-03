#pragma once

#include <cstddef>
#include <memory>

namespace fheco::ir
{
class Func;
} // namespace fheco::ir

namespace fheco::passes
{
// CKKS only. Wrap every ciphertext multiplication (ciphertext-ciphertext,
// ciphertext-plaintext, square) in a rescale term, after its relinearization
// when there is one, giving mul -> relin -> rescale. Must run after
// relin_after_ctxt_ctxt_mul. Returns the number of rescale terms inserted.
std::size_t insert_rescale(const std::shared_ptr<ir::Func> &func);

// CKKS only. Assign every ciphertext term a level (the number of rescale and
// mod_switch terms on its path from a fresh input) and fail with
// std::logic_error on what is not supported yet: a binary operation whose
// ciphertext operands are at different levels (no automatic level alignment)
// and a program consuming more levels than the coefficient modulus chain
// provides. Returns the maximum number of levels consumed.
std::size_t check_ckks_levels(const std::shared_ptr<ir::Func> &func);
} // namespace fheco::passes
