void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");

  Ciphertext z = x * alpha;
  Ciphertext z2 = z * z;
  Ciphertext z3 = z * z2;
  Ciphertext z5 = z3 * z2;

  Ciphertext z_2 = z + z;
  Ciphertext z_3 = z_2 + z;
  Ciphertext z_4 = z_2 + z_2;
  Ciphertext z_5 = z_4 + z;

  Ciphertext z3_2 = z3 + z3;
  Ciphertext z3_4 = z3_2 + z3_2;
  Ciphertext z3_16 = z3_4 + z3_4;
  z3_16 = z3_16 + z3_16;
  Ciphertext z3_20 = z3_16 + z3_4;

  Ciphertext z5_2 = z5 + z5;
  Ciphertext z5_4 = z5_2 + z5_2;
  Ciphertext z5_8 = z5_4 + z5_4;
  Ciphertext z5_16 = z5_8 + z5_8;

  Ciphertext T3 = z3_4 - z_3;
  Ciphertext T5 = (z5_16 - z3_20) + z_5;

  Ciphertext result = (z * c1 + c0) + T3 * c3 + T5 * c5;
  result.set_output("y");
}
