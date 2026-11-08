void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext zz = z + z;
  Ciphertext g2 = zz * c2;
  Ciphertext m = g2 + c1;
  Ciphertext r = m * z;
  r = r + c0;
  r = r - c2;
  r.set_output("y");
}
