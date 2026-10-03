#include "fheco/ir/func.hpp"
#include "fheco/ir/term.hpp"
#include "fheco/passes/insert_rescale.hpp"
#include <algorithm>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

namespace fheco::passes
{
namespace
{
  bool is_ctxt_mul(const ir::Term *term)
  {
    return term->type() == ir::Term::Type::cipher &&
           (term->op_code().type() == ir::OpCode::Type::mul || term->op_code().type() == ir::OpCode::Type::square);
  }

  void require_ckks(const shared_ptr<ir::Func> &func, const char *pass)
  {
    if (func->scheme() != Scheme::ckks)
      throw logic_error(string(pass) + " must only run on CKKS functions");
  }
} // namespace

size_t insert_rescale(const shared_ptr<ir::Func> &func)
{
  require_ckks(func, "insert_rescale");

  // Copy: the pass inserts terms, which changes the topological order.
  const vector<size_t> ids = func->get_top_sorted_terms_ids();
  size_t count = 0;
  for (auto id : ids)
  {
    auto term = func->data_flow().get_term(id);
    if (!term || !is_ctxt_mul(term))
      continue;

    // relin_after_ctxt_ctxt_mul redirects every user of a ciphertext product
    // to relin(product), so a relinearized product has exactly that parent.
    ir::Term *target = term;
    if (term->parents().size() == 1)
    {
      auto parent = *term->parents().begin();
      if (parent->op_code().type() == ir::OpCode::Type::relin)
        target = parent;
    }

    auto rescale_term = func->insert_op_term(ir::OpCode::rescale, {target});
    func->replace_term_with(target, rescale_term);
    ++count;
  }
  return count;
}

size_t check_ckks_levels(const shared_ptr<ir::Func> &func)
{
  require_ckks(func, "check_ckks_levels");

  const auto &bit_sizes = func->ckks_params().coeff_mod_bit_sizes;
  // Fresh ciphertexts use every prime except the special one; each rescale
  // drops one, and the first prime must remain.
  const size_t available_levels = bit_sizes.size() - 2;

  unordered_map<const ir::Term *, size_t> levels;
  size_t max_level = 0;
  for (auto term : func->get_top_sorted_terms())
  {
    if (term->type() != ir::Term::Type::cipher)
      continue;

    if (term->is_leaf())
    {
      levels.emplace(term, 0);
      continue;
    }

    const bool is_match_scale = term->op_code().type() == ir::OpCode::Type::match_scale;
    size_t level = 0;
    bool first = true;
    for (size_t i = 0; i < term->operands().size(); ++i)
    {
      auto operand = term->operands()[i];
      // match_scale's second operand only gives the target scale
      if (operand->type() != ir::Term::Type::cipher || (is_match_scale && i == 1))
        continue;

      auto operand_level = levels.at(operand);
      if (!first && operand_level != level)
        throw logic_error(
          "CKKS level mismatch: ciphertext operands of '" + term->op_code().str_repr() + "' (term " +
          to_string(term->id()) + ") are at levels " + to_string(level) + " and " + to_string(operand_level) +
          "; they should have been aligned by align_ckks_operands");

      level = operand_level;
      first = false;
    }

    if (
      term->op_code().type() == ir::OpCode::Type::rescale || term->op_code().type() == ir::OpCode::Type::mod_switch ||
      is_match_scale)
      ++level;

    if (level > available_levels)
      throw logic_error(
        "CKKS program needs " + to_string(level) + " levels but the coefficient modulus chain provides " +
        to_string(available_levels));

    levels.emplace(term, level);
    max_level = max(max_level, level);
  }
  return max_level;
}
} // namespace fheco::passes
