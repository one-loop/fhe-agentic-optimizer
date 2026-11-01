void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext s = square(z);
  Ciphertext q = s * c2;
  Ciphertext q2 = q + q;
  Ciphertext l = z * c1;
  Ciphertext r = q2 + l;
  r = r + c0;
  r = r - c2;
  r.set_output("y");
}
