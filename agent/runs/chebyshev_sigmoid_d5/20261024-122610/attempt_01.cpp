void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Ciphertext z = x * alpha;
  Ciphertext u = z * z;
  Ciphertext t2 = u + u - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t4s = t2 * t2;
  Ciphertext t4 = t4s + t4s - one;
  Ciphertext zt4 = z * t4;
  Ciphertext t5 = zt4 + zt4 - t3;
  Ciphertext r = z * c1 + t3 * c3 + t5 * c5 + c0;
  r.set_output("y");
}
