void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext z2 = z + z;
  Ciphertext v = z2 * c2 + c1;
  Ciphertext w = z * v;
  Ciphertext result = w - c2;
  result = result + c0;
  result.set_output("y");
}
