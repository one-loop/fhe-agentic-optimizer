#pragma once

// Runtime helpers for CHEHAB-generated CKKS programs (he/main.cpp written by
// gen_main_code_ckks). The BFV counterpart is benchmarks/utils.hpp.

#include <cstddef>
#include <istream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "seal/seal.h"

using EncryptedArgs = std::unordered_map<std::string, seal::Ciphertext>;
using EncodedArgs = std::unordered_map<std::string, seal::Plaintext>;
using CkksClearArgs = std::unordered_map<std::string, std::vector<double>>;

// Contents of an fhe_io_example file with real values. Same layout as the BFV
// files:
//   <func_slot_count> <nb_inputs> <nb_outputs>
//   <label> <is_cipher> <is_signed> <v_0> ... <v_{func_slot_count-1}>   (inputs)
//   <label> <is_cipher> <v_0> ... <v_{func_slot_count-1}>               (outputs)
// is_signed is ignored for CKKS.
// A plaintext input line may instead give a single value: a scalar operand,
// encoded as a constant in every slot.
struct CkksIoExample
{
  std::size_t func_slot_count = 0;
  CkksClearArgs cipher_inputs;
  CkksClearArgs plain_inputs;
  CkksClearArgs outputs;
};

CkksIoExample parse_ckks_io_file(std::istream &is);

// Repeat every input's values to fill all slots. Ciphertext inputs are then
// encoded at `scale` and encrypted. Plaintext inputs are returned as real
// values: the generated fhe() encodes them where they are used, at the level
// and scale of the ciphertext they meet. Scalar plaintext inputs (one value)
// are returned as that one value.
void prepare_ckks_inputs(
  const seal::CKKSEncoder &encoder, const seal::Encryptor &encryptor, double scale, const CkksIoExample &io,
  EncryptedArgs &encrypted_inputs, CkksClearArgs &plain_inputs);

// Decrypt and decode every output, keeping the first func_slot_count slots.
CkksClearArgs decrypt_ckks_outputs(
  const seal::CKKSEncoder &encoder, seal::Decryptor &decryptor, const EncryptedArgs &encrypted_outputs,
  std::size_t func_slot_count);

// Compare obtained with expected outputs: max |obtained - expected| <= tolerance
// for every expected output. Prints, per output, the error, the chain index of
// the fresh inputs and of the output, the levels consumed and the output scale.
bool check_ckks_outputs(
  const seal::SEALContext &context, const EncryptedArgs &encrypted_inputs, const EncryptedArgs &encrypted_outputs,
  const CkksClearArgs &expected, const CkksClearArgs &obtained, double tolerance, std::ostream &os);

double median(std::vector<double> values);
