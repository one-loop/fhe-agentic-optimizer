void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");

  Ciphertext z = x * alpha;
  Ciphertext u = z * z;
  Ciphertext s = u + u - one;
  Ciphertext q = s * s;
  Ciphertext t4 = q + q - one;
  Ciphertext c6s = s * c6;
  Ciphertext d = c6s + c6s + c4;
  Ciphertext c2s = s * c2;
  Ciphertext lin = z * c1;
  Ciphertext main_term = t4 * d;
  Ciphertext low = c2s - c6s;
  Ciphertext result = main_term + low + lin + c0;
  result.set_output("y");
}
