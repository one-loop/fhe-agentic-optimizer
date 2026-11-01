void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext v = z * c2;
  Ciphertext w = (v + v) + c1;
  Ciphertext m = z * w;
  Ciphertext r = m - c2;
  r = r + c0;
  r.set_output("y");
}
