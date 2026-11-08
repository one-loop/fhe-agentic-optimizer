void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext p = x * c2;
  Ciphertext p2 = p + p;
  Ciphertext s = z * z;
  Ciphertext a = z * c1;
  Ciphertext u = p2 * alpha;
  Ciphertext v = u * z;
  Ciphertext r = v + a;
  r = r + c0;
  r = r - c2;
  r.set_output("y");
}
