void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext q = square(z);
  Ciphertext t = (q + q) * c2;
  Ciphertext l = z * c1;
  Ciphertext y = t + l - c2 + c0;
  y.set_output("y");
}
