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
Ciphertext c16 = encrypted_inputs.at("x_3_3");
Ciphertext c15 = encrypted_inputs.at("x_3_2");
Ciphertext c14 = encrypted_inputs.at("x_3_1");
Ciphertext c13 = encrypted_inputs.at("x_3_0");
Ciphertext c12 = encrypted_inputs.at("x_2_3");
Ciphertext c11 = encrypted_inputs.at("x_2_2");
Ciphertext c10 = encrypted_inputs.at("x_2_1");
Ciphertext c9 = encrypted_inputs.at("x_2_0");
Ciphertext c8 = encrypted_inputs.at("x_1_3");
Ciphertext c7 = encrypted_inputs.at("x_1_2");
Ciphertext c6 = encrypted_inputs.at("x_1_1");
Ciphertext c5 = encrypted_inputs.at("x_1_0");
Ciphertext c4 = encrypted_inputs.at("x_0_3");
Ciphertext c3 = encrypted_inputs.at("x_0_2");
Ciphertext c2 = encrypted_inputs.at("x_0_1");
Ciphertext c1 = encrypted_inputs.at("x_0_0");
evaluator.add(c11, c12, c11);
evaluator.add(c15, c11, c15);
evaluator.add(c16, c15, c16);
evaluator.add(c9, c10, c9);
evaluator.add(c13, c9, c13);
evaluator.add(c14, c13, c14);
evaluator.add(c3, c4, c3);
evaluator.add(c7, c3, c7);
evaluator.add(c8, c7, c8);
evaluator.add(c1, c2, c1);
evaluator.add(c5, c1, c5);
evaluator.add(c6, c5, c6);
encrypted_outputs.emplace("y_1_1", move(c16));
encrypted_outputs.emplace("y_1_0", move(c14));
encrypted_outputs.emplace("y_0_1", move(c8));
encrypted_outputs.emplace("y_0_0", move(c6));
}

vector<int> get_rotation_steps_fhe(){
return vector<int>{};
}

