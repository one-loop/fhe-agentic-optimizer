void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext m = z * c2;
  Ciphertext m2 = m + m;
  Ciphertext s = m2 + c1;
  Ciphertext p = s * z;
  Ciphertext r = p + c0;
  Ciphertext y = r - c2;
  y.set_output("y");
}
