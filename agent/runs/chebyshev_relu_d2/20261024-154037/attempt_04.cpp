void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext m = z * c2;
  Ciphertext s = m + m;
  s = s + c1;
  Ciphertext p = s * z;
  Ciphertext y = (p + c0) - c2;
  y.set_output("y");
}
