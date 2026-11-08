void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext s = square(z);
  Ciphertext t = s * c2;
  Ciphertext t2 = t + t;
  Ciphertext h = z * c1;
  Ciphertext r = t2 + h;
  r = r - c2;
  r = r + c0;
  r.set_output("y");
}
