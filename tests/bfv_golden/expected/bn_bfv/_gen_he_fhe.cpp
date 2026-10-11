#include <cstddef>
#include <cstdint>
#include <utility>
#include "_gen_he_fhe.hpp"

using namespace std;
using namespace seal;

void fhe(const unordered_map<string, Ciphertext> &encrypted_inputs,
const unordered_map<string, Plaintext> &encoded_inputs,
unordered_map<string, Ciphertext> &encrypted_outputs,
unordered_map<string, Plaintext> &encoded_outputs,
const BatchEncoder &encoder,
const Encryptor &encryptor,
const Evaluator &evaluator,
const RelinKeys &relin_keys,
const GaloisKeys &galois_keys)
{
Plaintext p3 = encoded_inputs.at("B");
Plaintext p2 = encoded_inputs.at("A");
Ciphertext c1 = encrypted_inputs.at("x");
evaluator.multiply_plain(c1, p2, c1);
evaluator.add_plain(c1, p3, c1);
encrypted_outputs.emplace("y", move(c1));
}

vector<int> get_rotation_steps_fhe(){
return vector<int>{};
}

