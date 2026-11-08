void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext a = x * alpha;
  Ciphertext b = x * c2;
  Ciphertext p = a * b;
  Ciphertext p2 = p + p;
  Ciphertext t1 = a * c1;
  Ciphertext r = p2 + t1;
  r = r - c2;
  r = r + c0;
  r.set_output("y");
}
