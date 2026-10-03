#pragma once

#include <cstddef>
#include <memory>

namespace fheco::ir
{
class Func;
} // namespace fheco::ir

namespace fheco::passes
{
// CKKS only. Run after insert_rescale. Tracks, for every ciphertext term, its
// level and its scale symbolically (as Delta^a * prod_l q_l^-b_l, Delta the
// nominal scale and q_l the prime dropped by a rescale at level l), and
// rebuilds binary operations whose ciphertext operands do not line up:
//
// - mul: the operand at the higher level is mod-switched down;
// - add/sub with equal scales: the operand at the higher level is
//   mod-switched down;
// - add/sub with different scales: the operand at the higher level goes
//   through match_scale(hi, lo), landing exactly on the other operand's scale
//   one level lower, then is mod-switched down;
// - add/sub at the same level with different scales: std::logic_error (it
//   would need an extra level).
//
// Ciphertexts are never raised to a higher level. Returns the number of
// mod_switch and match_scale terms inserted.
std::size_t align_ckks_operands(const std::shared_ptr<ir::Func> &func);
} // namespace fheco::passes
