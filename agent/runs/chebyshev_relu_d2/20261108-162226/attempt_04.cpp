void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext zz = z + z;
  Ciphertext u = zz * c2;
  Ciphertext v = u + c1;
  Ciphertext w = z * v;
  Ciphertext result = w + c0;
  result = result - c2;
  result.set_output("y");
}
