void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext q = z2 * c2;
  Ciphertext l = z * c1;
  Ciphertext r = (q + q) + l;
  Ciphertext y = (r - c2) + c0;
  y.set_output("y");
}
