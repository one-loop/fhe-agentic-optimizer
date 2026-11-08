void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext w = z * c2;
  Ciphertext u = (w + w) + c1;
  Ciphertext p = u * z;
  Ciphertext y = (p - c2) + c0;
  y.set_output("y");
}
