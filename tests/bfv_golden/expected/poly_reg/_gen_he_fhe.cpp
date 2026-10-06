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
Ciphertext c17 = encrypted_inputs.at("c4_0");
Ciphertext c16 = encrypted_inputs.at("c3_3");
Ciphertext c38 = encrypted_inputs.at("c4_3");
Ciphertext c15 = encrypted_inputs.at("c3_2");
Ciphertext c14 = encrypted_inputs.at("c3_1");
Ciphertext c13 = encrypted_inputs.at("c3_0");
Ciphertext c12 = encrypted_inputs.at("c2_3");
Ciphertext c11 = encrypted_inputs.at("c2_2");
Ciphertext c10 = encrypted_inputs.at("c2_1");
Ciphertext c9 = encrypted_inputs.at("c2_0");
Ciphertext c31 = encrypted_inputs.at("c4_2");
Ciphertext c8 = encrypted_inputs.at("c1_3");
Ciphertext c7 = encrypted_inputs.at("c1_2");
Ciphertext c6 = encrypted_inputs.at("c1_1");
Ciphertext c5 = encrypted_inputs.at("c1_0");
Ciphertext c4 = encrypted_inputs.at("c0_3");
Ciphertext c3 = encrypted_inputs.at("c0_2");
Ciphertext c2 = encrypted_inputs.at("c0_1");
Ciphertext c24 = encrypted_inputs.at("c4_1");
Ciphertext c1 = encrypted_inputs.at("c0_0");
evaluator.multiply(c4, c38, c38);
evaluator.relinearize(c38, relin_keys, c38);
evaluator.multiply(c4, c38, c38);
evaluator.relinearize(c38, relin_keys, c38);
evaluator.multiply(c4, c16, c4);
evaluator.relinearize(c4, relin_keys, c4);
evaluator.add(c38, c4, c38);
evaluator.add(c12, c38, c12);
evaluator.add(c8, c12, c8);
evaluator.multiply(c3, c31, c31);
evaluator.relinearize(c31, relin_keys, c31);
evaluator.multiply(c3, c31, c31);
evaluator.relinearize(c31, relin_keys, c31);
evaluator.multiply(c3, c15, c3);
evaluator.relinearize(c3, relin_keys, c3);
evaluator.add(c31, c3, c31);
evaluator.add(c11, c31, c11);
evaluator.add(c7, c11, c7);
evaluator.multiply(c2, c24, c24);
evaluator.relinearize(c24, relin_keys, c24);
evaluator.multiply(c2, c24, c24);
evaluator.relinearize(c24, relin_keys, c24);
evaluator.multiply(c2, c14, c2);
evaluator.relinearize(c2, relin_keys, c2);
evaluator.add(c24, c2, c24);
evaluator.add(c10, c24, c10);
evaluator.add(c6, c10, c6);
evaluator.multiply(c1, c17, c17);
evaluator.relinearize(c17, relin_keys, c17);
evaluator.multiply(c1, c17, c17);
evaluator.relinearize(c17, relin_keys, c17);
evaluator.multiply(c1, c13, c1);
evaluator.relinearize(c1, relin_keys, c1);
evaluator.add(c17, c1, c17);
evaluator.add(c9, c17, c9);
evaluator.add(c5, c9, c5);
encrypted_outputs.emplace("c_result_3", move(c8));
encrypted_outputs.emplace("c_result_2", move(c7));
encrypted_outputs.emplace("c_result_1", move(c6));
encrypted_outputs.emplace("c_result_0", move(c5));
}

vector<int> get_rotation_steps_fhe(){
return vector<int>{};
}

