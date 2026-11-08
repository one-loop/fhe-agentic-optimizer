void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t = z2 + z2;
  Ciphertext r = t * c2;
  Ciphertext a = z * c1;
  r = r + a;
  r = r + c0;
  r = r - c2;
  r.set_output("y");
}
