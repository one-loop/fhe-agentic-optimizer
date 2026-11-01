void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;          // level 1
  Ciphertext w = z * c2;             // level 2: c2*z
  Ciphertext inner = w + w + c1;     // 2*c2*z + c1
  Ciphertext prod = inner * x;       // level 3: x*(2c2 z + c1)
  Ciphertext scaled = prod * alpha;  // level 4: z*(2c2 z + c1)
  Ciphertext result = scaled + c0 - c2;
  result.set_output("y");
}
