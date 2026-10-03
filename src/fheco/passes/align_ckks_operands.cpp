#include "fheco/ir/func.hpp"
#include "fheco/ir/term.hpp"
#include "fheco/passes/align_ckks_operands.hpp"
#include <algorithm>
#include <map>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

namespace fheco::passes
{
namespace
{
  // Delta^delta_exp * prod_l q_l^q_exps[l], with zero exponents omitted so that
  // equal scales compare equal.
  struct SymbolicScale
  {
    int delta_exp = 0;
    map<size_t, int> q_exps;

    SymbolicScale operator*(const SymbolicScale &other) const
    {
      SymbolicScale result = *this;
      result.delta_exp += other.delta_exp;
      for (const auto &[level, exp] : other.q_exps)
        result.add_q_exp(level, exp);
      return result;
    }

    void add_q_exp(size_t level, int exp)
    {
      if ((q_exps[level] += exp) == 0)
        q_exps.erase(level);
    }

    bool operator==(const SymbolicScale &other) const
    {
      return delta_exp == other.delta_exp && q_exps == other.q_exps;
    }
  };

  struct CtxtInfo
  {
    // number of levels consumed
    size_t level;
    SymbolicScale scale;
  };

  const SymbolicScale delta{1, {}};
} // namespace

size_t align_ckks_operands(const shared_ptr<ir::Func> &func)
{
  if (func->scheme() != Scheme::ckks)
    throw logic_error("align_ckks_operands must only run on CKKS functions");

  unordered_map<const ir::Term *, CtxtInfo> infos;
  size_t inserted = 0;

  // Insert (or reuse, through CSE) a term and record its info.
  auto insert = [&](ir::OpCode op_code, vector<ir::Term *> operands, CtxtInfo info) {
    auto term = func->insert_op_term(move(op_code), move(operands));
    if (infos.emplace(term, move(info)).second)
      ++inserted;
    return term;
  };

  auto mod_switch_to = [&](ir::Term *term, size_t level) {
    while (infos.at(term).level < level)
    {
      auto info = infos.at(term);
      ++info.level;
      term = insert(ir::OpCode::mod_switch, {term}, move(info));
    }
    return term;
  };

  // Copy: the pass inserts and replaces terms.
  const vector<size_t> ids = func->get_top_sorted_terms_ids();
  for (auto id : ids)
  {
    auto term = func->data_flow().get_term(id);
    if (!term || term->type() != ir::Term::Type::cipher)
      continue;

    if (term->is_leaf())
    {
      infos.emplace(term, CtxtInfo{0, delta});
      continue;
    }

    const auto op_type = term->op_code().type();
    const auto &operands = term->operands();
    switch (op_type)
    {
    case ir::OpCode::Type::relin:
    case ir::OpCode::Type::negate:
    case ir::OpCode::Type::rotate:
      infos.emplace(term, infos.at(operands[0]));
      break;

    case ir::OpCode::Type::mod_switch:
    {
      auto info = infos.at(operands[0]);
      ++info.level;
      infos.emplace(term, move(info));
      break;
    }

    case ir::OpCode::Type::rescale:
    {
      auto info = infos.at(operands[0]);
      info.scale.add_q_exp(info.level, -1);
      ++info.level;
      infos.emplace(term, move(info));
      break;
    }

    case ir::OpCode::Type::square:
    {
      const auto &info = infos.at(operands[0]);
      infos.emplace(term, CtxtInfo{info.level, info.scale * info.scale});
      break;
    }

    case ir::OpCode::Type::mul:
    case ir::OpCode::Type::add:
    case ir::OpCode::Type::sub:
    {
      const bool lhs_cipher = operands[0]->type() == ir::Term::Type::cipher;
      const bool rhs_cipher = operands[1]->type() == ir::Term::Type::cipher;
      if (!lhs_cipher || !rhs_cipher)
      {
        // ciphertext-plaintext: the plaintext is encoded at the ciphertext's
        // level, with the nominal scale for mul and the exact scale otherwise
        auto info = infos.at(lhs_cipher ? operands[0] : operands[1]);
        if (op_type == ir::OpCode::Type::mul)
          info.scale = info.scale * delta;
        infos.emplace(term, move(info));
        break;
      }

      ir::Term *lhs = operands[0];
      ir::Term *rhs = operands[1];
      const auto lhs_info = infos.at(lhs);
      const auto rhs_info = infos.at(rhs);
      CtxtInfo result_info;
      if (op_type == ir::OpCode::Type::mul)
      {
        const size_t level = max(lhs_info.level, rhs_info.level);
        lhs = mod_switch_to(lhs, level);
        rhs = mod_switch_to(rhs, level);
        result_info = CtxtInfo{level, lhs_info.scale * rhs_info.scale};
      }
      else
      {
        if (lhs_info.level == rhs_info.level && !(lhs_info.scale == rhs_info.scale))
          throw logic_error(
            "CKKS scale mismatch: ciphertext operands of '" + term->op_code().str_repr() + "' (term " +
            to_string(term->id()) + ") are at the same level " + to_string(lhs_info.level) +
            " with different scales; aligning them would need an extra level");

        // hi: the operand with more levels left (fewer consumed)
        const bool lhs_is_hi = lhs_info.level < rhs_info.level;
        ir::Term *&hi = lhs_is_hi ? lhs : rhs;
        ir::Term *lo = lhs_is_hi ? rhs : lhs;
        const auto &lo_info = lhs_is_hi ? rhs_info : lhs_info;
        const auto &hi_info = lhs_is_hi ? lhs_info : rhs_info;
        if (!(hi_info.scale == lo_info.scale))
          hi = insert(ir::OpCode::match_scale, {hi, lo}, CtxtInfo{hi_info.level + 1, lo_info.scale});
        hi = mod_switch_to(hi, lo_info.level);
        result_info = lo_info;
      }

      if (lhs != operands[0] || rhs != operands[1])
      {
        auto aligned = func->insert_op_term(term->op_code(), {lhs, rhs});
        func->replace_term_with(term, aligned);
        infos.emplace(aligned, result_info);
      }
      else
        infos.emplace(term, result_info);
      break;
    }

    default:
      throw logic_error(
        "align_ckks_operands: unhandled ciphertext operation '" + term->op_code().str_repr() + "'");
    }
  }
  return inserted;
}
} // namespace fheco::passes
