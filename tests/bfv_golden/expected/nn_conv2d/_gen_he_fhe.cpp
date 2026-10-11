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
Ciphertext c26 = encrypted_inputs.at("b");
Ciphertext c25 = encrypted_inputs.at("k_2_2");
Ciphertext c24 = encrypted_inputs.at("k_2_1");
Ciphertext c23 = encrypted_inputs.at("k_2_0");
Ciphertext c22 = encrypted_inputs.at("k_1_2");
Ciphertext c21 = encrypted_inputs.at("k_1_1");
Ciphertext c20 = encrypted_inputs.at("k_1_0");
Ciphertext c19 = encrypted_inputs.at("k_0_2");
Ciphertext c18 = encrypted_inputs.at("k_0_1");
Ciphertext c17 = encrypted_inputs.at("k_0_0");
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
Ciphertext c81;
evaluator.multiply(c6, c17, c81);
evaluator.relinearize(c81, relin_keys, c81);
evaluator.add(c26, c81, c81);
Ciphertext c83;
evaluator.multiply(c7, c18, c83);
evaluator.relinearize(c83, relin_keys, c83);
evaluator.add(c81, c83, c81);
evaluator.multiply(c8, c19, c83);
evaluator.relinearize(c83, relin_keys, c83);
evaluator.add(c81, c83, c81);
evaluator.multiply(c10, c20, c83);
evaluator.relinearize(c83, relin_keys, c83);
evaluator.add(c81, c83, c81);
evaluator.multiply(c11, c21, c83);
evaluator.relinearize(c83, relin_keys, c83);
evaluator.add(c81, c83, c81);
evaluator.multiply(c12, c22, c83);
evaluator.relinearize(c83, relin_keys, c83);
evaluator.add(c81, c83, c81);
evaluator.multiply(c14, c23, c83);
evaluator.relinearize(c83, relin_keys, c83);
evaluator.add(c81, c83, c81);
evaluator.multiply(c15, c24, c83);
evaluator.relinearize(c83, relin_keys, c83);
evaluator.add(c81, c83, c81);
evaluator.multiply(c16, c25, c16);
evaluator.relinearize(c16, relin_keys, c16);
evaluator.add(c81, c16, c81);
evaluator.multiply(c5, c17, c83);
evaluator.relinearize(c83, relin_keys, c83);
evaluator.add(c26, c83, c83);
evaluator.multiply(c6, c18, c16);
evaluator.relinearize(c16, relin_keys, c16);
evaluator.add(c83, c16, c83);
evaluator.multiply(c7, c19, c16);
evaluator.relinearize(c16, relin_keys, c16);
evaluator.add(c83, c16, c83);
evaluator.multiply(c9, c20, c16);
evaluator.relinearize(c16, relin_keys, c16);
evaluator.add(c83, c16, c83);
evaluator.multiply(c10, c21, c16);
evaluator.relinearize(c16, relin_keys, c16);
evaluator.add(c83, c16, c83);
evaluator.multiply(c11, c22, c16);
evaluator.relinearize(c16, relin_keys, c16);
evaluator.add(c83, c16, c83);
evaluator.multiply(c13, c23, c13);
evaluator.relinearize(c13, relin_keys, c13);
evaluator.add(c83, c13, c83);
evaluator.multiply(c14, c24, c14);
evaluator.relinearize(c14, relin_keys, c14);
evaluator.add(c83, c14, c83);
evaluator.multiply(c15, c25, c15);
evaluator.relinearize(c15, relin_keys, c15);
evaluator.add(c83, c15, c83);
evaluator.multiply(c2, c17, c16);
evaluator.relinearize(c16, relin_keys, c16);
evaluator.add(c26, c16, c16);
evaluator.multiply(c3, c18, c13);
evaluator.relinearize(c13, relin_keys, c13);
evaluator.add(c16, c13, c16);
evaluator.multiply(c4, c19, c4);
evaluator.relinearize(c4, relin_keys, c4);
evaluator.add(c16, c4, c16);
evaluator.multiply(c6, c20, c14);
evaluator.relinearize(c14, relin_keys, c14);
evaluator.add(c16, c14, c16);
evaluator.multiply(c7, c21, c15);
evaluator.relinearize(c15, relin_keys, c15);
evaluator.add(c16, c15, c16);
evaluator.multiply(c8, c22, c8);
evaluator.relinearize(c8, relin_keys, c8);
evaluator.add(c16, c8, c16);
evaluator.multiply(c10, c23, c8);
evaluator.relinearize(c8, relin_keys, c8);
evaluator.add(c16, c8, c16);
evaluator.multiply(c11, c24, c8);
evaluator.relinearize(c8, relin_keys, c8);
evaluator.add(c16, c8, c16);
evaluator.multiply(c12, c25, c12);
evaluator.relinearize(c12, relin_keys, c12);
evaluator.add(c16, c12, c16);
evaluator.multiply(c1, c17, c1);
evaluator.relinearize(c1, relin_keys, c1);
evaluator.add(c26, c1, c26);
evaluator.multiply(c2, c18, c2);
evaluator.relinearize(c2, relin_keys, c2);
evaluator.add(c26, c2, c26);
evaluator.multiply(c3, c19, c3);
evaluator.relinearize(c3, relin_keys, c3);
evaluator.add(c26, c3, c26);
evaluator.multiply(c5, c20, c5);
evaluator.relinearize(c5, relin_keys, c5);
evaluator.add(c26, c5, c26);
evaluator.multiply(c6, c21, c6);
evaluator.relinearize(c6, relin_keys, c6);
evaluator.add(c26, c6, c26);
evaluator.multiply(c7, c22, c7);
evaluator.relinearize(c7, relin_keys, c7);
evaluator.add(c26, c7, c26);
evaluator.multiply(c9, c23, c9);
evaluator.relinearize(c9, relin_keys, c9);
evaluator.add(c26, c9, c26);
evaluator.multiply(c10, c24, c10);
evaluator.relinearize(c10, relin_keys, c10);
evaluator.add(c26, c10, c26);
evaluator.multiply(c11, c25, c11);
evaluator.relinearize(c11, relin_keys, c11);
evaluator.add(c26, c11, c26);
encrypted_outputs.emplace("y_1_1", move(c81));
encrypted_outputs.emplace("y_1_0", move(c83));
encrypted_outputs.emplace("y_0_1", move(c16));
encrypted_outputs.emplace("y_0_0", move(c26));
}

vector<int> get_rotation_steps_fhe(){
return vector<int>{};
}

