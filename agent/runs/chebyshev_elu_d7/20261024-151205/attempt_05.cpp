void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext zz = square(z);
  Ciphertext t2 = zz + zz - one;
  Ciphertext p3 = z * t2;
  Ciphertext t3 = p3 + p3 - z;
  Ciphertext q = square(t2);
  Ciphertext t4 = q + q - one;

  Ciphertext m6 = t2 * c6;
  Ciphertext s = z * c5;
  s = s + m6;
  s = s + t3 * c7;
  Ciphertext bc = (s + s) + c4;
  Ciphertext top = t4 * bc;

  Ciphertext a = z * c1 - z * c7;
  a = a + (t2 * c2 - m6);
  a = a + (t3 * c3 - t3 * c5);
  Ciphertext y = (top + a) + c0;
  y.set_output("y");
}
