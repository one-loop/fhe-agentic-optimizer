#pragma once

#include "fheco/ir/common.hpp"
#include <cstddef>
#include <string_view>
#include <unordered_map>

using namespace std::literals;

namespace fheco::code_gen
{
constexpr std::string_view seal_namespace{"seal"};

constexpr std::string_view cipher_type{"Ciphertext"};
constexpr std::string_view plain_type{"Plaintext"};

constexpr std::string_view header_encrypted_io_type{"std::unordered_map<std::string, seal::Ciphertext>"};
constexpr std::string_view header_encoded_io_type{"std::unordered_map<std::string, seal::Plaintext>"};

constexpr std::string_view source_encrypted_io_type{"unordered_map<string, Ciphertext>"};
constexpr std::string_view source_encoded_io_type{"unordered_map<string, Plaintext>"};

constexpr std::string_view encrypted_inputs_container_id{"encrypted_inputs"};
constexpr std::string_view encoded_inputs_container_id{"encoded_inputs"};

// CKKS: plaintext inputs reach fhe() as real values and are encoded at each use,
// at the level and scale of the ciphertext they meet.
constexpr std::string_view header_ckks_plain_io_type{"std::unordered_map<std::string, std::vector<double>>"};
constexpr std::string_view source_ckks_plain_io_type{"unordered_map<string, vector<double>>"};
constexpr std::string_view ckks_plain_inputs_container_id{"plain_inputs"};
constexpr std::string_view ckks_scale_id{"scale"};
constexpr std::string_view context_type{"SEALContext"};
constexpr std::string_view context_id{"context"};

// Helpers emitted into generated CKKS sources that align scales.
constexpr std::string_view ckks_scale_helpers{
  R"(namespace
{
// Plaintext scale that makes multiply_plain followed by rescale_to_next land
// exactly on target_scale.
[[maybe_unused]] double ckks_landing_scale(const SEALContext &context, const Ciphertext &ct, double target_scale)
{
  const double q =
    static_cast<double>(context.get_context_data(ct.parms_id())->parms().coeff_modulus().back().value());
  return target_scale * q / ct.scale();
}

// Assign `scale` to ct only when the two agree up to floating-point rounding
// (1e-9 relative); a real mismatch is an error, never silently overwritten.
[[maybe_unused]] void ckks_snap_scale(Ciphertext &ct, double scale)
{
  if (std::abs(ct.scale() - scale) > 1e-9 * std::max(std::abs(ct.scale()), std::abs(scale)))
    throw std::logic_error("CKKS scale mismatch");
  ct.scale() = scale;
}
} // namespace
)"};

constexpr std::string_view encrypted_outputs_container_id{"encrypted_outputs"};
constexpr std::string_view encoded_outputs_container_id{"encoded_outputs"};

constexpr std::string_view relin_keys_type{"RelinKeys"};
constexpr std::string_view relin_keys_id{"relin_keys"};

constexpr std::string_view galois_keys_type{"GaloisKeys"};
constexpr std::string_view galois_keys_id{"galois_keys"};

const std::string rotation_steps_getter_id = "get_rotation_steps";
constexpr std::size_t line_threshold = 16;

constexpr std::string_view encoder_type{"BatchEncoder"};
constexpr std::string_view ckks_encoder_type{"CKKSEncoder"};
constexpr std::string_view encoder_id{"encoder"};
constexpr std::string_view slot_count_id{"slot_count"};
constexpr std::string_view encode{"encode"};
constexpr std::string_view decode{"decode"};

constexpr std::string_view encryptor_type{"Encryptor"};
constexpr std::string_view encryptor_id{"encryptor"};
constexpr std::string_view encrypt{"encrypt"};
constexpr std::string_view signed_slot_type{"std::int64_t"};
constexpr std::string_view unsigned_slot_type{"std::uint64_t"};

constexpr std::string_view evaluator_type{"Evaluator"};
constexpr std::string_view evaluator_id{"evaluator"};

constexpr std::string_view header_includes{
  R"(#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "seal/seal.h"
)"};

constexpr std::string_view source_includes{
  R"(#include <cstddef>
#include <cstdint>
#include <utility>
)"}; 

constexpr std::string_view source_usings{
  R"(using namespace std;
using namespace seal;
)"};

const std::unordered_map<ir::OpType, std::string_view, ir::HashOpType, ir::EqualOpType> operation_mapping = {
  {{ir::OpCode::Type::add, {ir::Term::Type::cipher, ir::Term::Type::cipher}}, "add"sv},
  {{ir::OpCode::Type::add, {ir::Term::Type::cipher, ir::Term::Type::plain}}, "add_plain"sv},
  {{ir::OpCode::Type::sub, {ir::Term::Type::cipher, ir::Term::Type::cipher}}, "sub"sv},
  {{ir::OpCode::Type::sub, {ir::Term::Type::cipher, ir::Term::Type::plain}}, "sub_plain"sv},
  {{ir::OpCode::Type::negate, {ir::Term::Type::cipher}}, "negate"sv},
  {{ir::OpCode::Type::rotate, {ir::Term::Type::cipher}}, "rotate_rows"sv},
  {{ir::OpCode::Type::square, {ir::Term::Type::cipher}}, "square"sv},
  {{ir::OpCode::Type::mul, {ir::Term::Type::cipher, ir::Term::Type::cipher}}, "multiply"sv},
  {{ir::OpCode::Type::mul, {ir::Term::Type::cipher, ir::Term::Type::plain}}, "multiply_plain"sv},
  {{ir::OpCode::Type::mod_switch, {ir::Term::Type::cipher}}, "mod_switch_to_next"sv},
  {{ir::OpCode::Type::relin, {ir::Term::Type::cipher}}, "relinearize"sv},
  {{ir::OpCode::Type::rescale, {ir::Term::Type::cipher}}, "rescale_to_next"sv},
};
} // namespace fheco::code_gen
