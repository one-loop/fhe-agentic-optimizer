void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext z2 = z + z;
  Ciphertext m = z2 * c2;
  Ciphertext s = m + c1;
  Ciphertext p = s * z;
  Ciphertext y = (p + c0) - c2;
  y.set_output("y");
}
