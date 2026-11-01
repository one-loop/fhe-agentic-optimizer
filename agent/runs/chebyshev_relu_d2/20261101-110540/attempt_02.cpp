void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;          // level 1
  Ciphertext z2 = z + z;             // 2z, level 1
  Ciphertext w = z2 * c2;            // 2*c2*z, level 2
  Ciphertext inner = w + c1;         // 2*c2*z + c1
  Ciphertext prod = inner * z;       // level 3
  Ciphertext result = prod - c2 + c0;
  result.set_output("y");
}
