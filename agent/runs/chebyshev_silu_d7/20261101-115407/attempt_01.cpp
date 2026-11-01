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
  Ciphertext w = z * z;
  Ciphertext u = w + w - one;          // T2
  Ciphertext u2 = u * u;
  Ciphertext t4 = u2 + u2 - one;       // T4
  Ciphertext s = u * c6;               // c6*T2
  Ciphertext v = s + s + c4;           // c4 + 2 c6 T2
  Ciphertext p = t4 * v;               // c4 T4 + c6 (T6 + T2)
  Ciphertext lin = z * c1;
  Ciphertext q = u * c2;
  Ciphertext result = p + (q - s) + lin + c0;
  result.set_output("y");
}
