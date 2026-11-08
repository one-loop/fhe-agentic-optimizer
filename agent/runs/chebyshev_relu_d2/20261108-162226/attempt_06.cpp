void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext a = x * c2;
  Ciphertext a2 = a + a;
  Ciphertext v = a2 * alpha + c1;
  Ciphertext z = x * alpha;
  Ciphertext w = z * v;
  Ciphertext result = w - c2;
  result = result + c0;
  result.set_output("y");
}
