void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;          // alpha x
  Ciphertext w = z * c2;             // c2 alpha x
  Ciphertext v = w + w + c1;         // 2 c2 z + c1
  Ciphertext p = x * v;              // x (2 c2 z + c1)
  Ciphertext q = p * alpha;          // z (2 c2 z + c1)
  Ciphertext y = q - c2 + c0;
  y.set_output("y");
}
