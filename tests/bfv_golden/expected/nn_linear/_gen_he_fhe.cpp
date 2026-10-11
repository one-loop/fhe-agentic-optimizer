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
Ciphertext c24 = encrypted_inputs.at("w_3_3");
Ciphertext c23 = encrypted_inputs.at("w_3_2");
Ciphertext c22 = encrypted_inputs.at("w_3_1");
Ciphertext c21 = encrypted_inputs.at("w_3_0");
Ciphertext c20 = encrypted_inputs.at("b_3");
Ciphertext c19 = encrypted_inputs.at("w_2_3");
Ciphertext c18 = encrypted_inputs.at("w_2_2");
Ciphertext c17 = encrypted_inputs.at("w_2_1");
Ciphertext c16 = encrypted_inputs.at("w_2_0");
Ciphertext c15 = encrypted_inputs.at("b_2");
Ciphertext c14 = encrypted_inputs.at("w_1_3");
Ciphertext c13 = encrypted_inputs.at("w_1_2");
Ciphertext c12 = encrypted_inputs.at("w_1_1");
Ciphertext c11 = encrypted_inputs.at("w_1_0");
Ciphertext c10 = encrypted_inputs.at("b_1");
Ciphertext c9 = encrypted_inputs.at("w_0_3");
Ciphertext c8 = encrypted_inputs.at("w_0_2");
Ciphertext c7 = encrypted_inputs.at("w_0_1");
Ciphertext c6 = encrypted_inputs.at("w_0_0");
Ciphertext c5 = encrypted_inputs.at("b_0");
Ciphertext c4 = encrypted_inputs.at("x_3");
Ciphertext c3 = encrypted_inputs.at("x_2");
Ciphertext c2 = encrypted_inputs.at("x_1");
Ciphertext c1 = encrypted_inputs.at("x_0");
evaluator.multiply(c1, c21, c21);
evaluator.relinearize(c21, relin_keys, c21);
evaluator.add(c20, c21, c20);
evaluator.multiply(c2, c22, c22);
evaluator.relinearize(c22, relin_keys, c22);
evaluator.add(c20, c22, c20);
evaluator.multiply(c3, c23, c23);
evaluator.relinearize(c23, relin_keys, c23);
evaluator.add(c20, c23, c20);
evaluator.multiply(c4, c24, c24);
evaluator.relinearize(c24, relin_keys, c24);
evaluator.add(c20, c24, c20);
evaluator.multiply(c1, c16, c16);
evaluator.relinearize(c16, relin_keys, c16);
evaluator.add(c15, c16, c15);
evaluator.multiply(c2, c17, c17);
evaluator.relinearize(c17, relin_keys, c17);
evaluator.add(c15, c17, c15);
evaluator.multiply(c3, c18, c18);
evaluator.relinearize(c18, relin_keys, c18);
evaluator.add(c15, c18, c15);
evaluator.multiply(c4, c19, c19);
evaluator.relinearize(c19, relin_keys, c19);
evaluator.add(c15, c19, c15);
evaluator.multiply(c1, c11, c11);
evaluator.relinearize(c11, relin_keys, c11);
evaluator.add(c10, c11, c10);
evaluator.multiply(c2, c12, c12);
evaluator.relinearize(c12, relin_keys, c12);
evaluator.add(c10, c12, c10);
evaluator.multiply(c3, c13, c13);
evaluator.relinearize(c13, relin_keys, c13);
evaluator.add(c10, c13, c10);
evaluator.multiply(c4, c14, c14);
evaluator.relinearize(c14, relin_keys, c14);
evaluator.add(c10, c14, c10);
evaluator.multiply(c1, c6, c1);
evaluator.relinearize(c1, relin_keys, c1);
evaluator.add(c5, c1, c5);
evaluator.multiply(c2, c7, c2);
evaluator.relinearize(c2, relin_keys, c2);
evaluator.add(c5, c2, c5);
evaluator.multiply(c3, c8, c3);
evaluator.relinearize(c3, relin_keys, c3);
evaluator.add(c5, c3, c5);
evaluator.multiply(c4, c9, c4);
evaluator.relinearize(c4, relin_keys, c4);
evaluator.add(c5, c4, c5);
encrypted_outputs.emplace("y_3", move(c20));
encrypted_outputs.emplace("y_2", move(c15));
encrypted_outputs.emplace("y_1", move(c10));
encrypted_outputs.emplace("y_0", move(c5));
}

vector<int> get_rotation_steps_fhe(){
return vector<int>{};
}

