void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext w = z * z;
  Ciphertext t2 = w + w - one;
  Ciphertext p = z * t2;
  Ciphertext t3 = p + p - z;
  Ciphertext q = t2 * t2;
  Ciphertext t4 = q + q - one;

  Ciphertext u5 = z * c5;
  Ciphertext u6 = t2 * c6;
  Ciphertext a7 = z * c7;
  Ciphertext r7 = p * c7;
  Ciphertext r5 = p * c5;
  Ciphertext u7 = (r7 + r7) - a7;
  Ciphertext a5 = (r5 + r5) - u5;
  Ciphertext B = u5 + u5 + u6 + u6 + u7 + u7 + c4;

  Ciphertext prod = t4 * B;

  Ciphertext a1 = z * c1;
  Ciphertext a2 = t2 * c2;
  Ciphertext a3 = t3 * c3;
  Ciphertext A = a1 - a7;
  A = A + a2;
  A = A - u6;
  A = A + a3;
  A = A - a5;
  A = A + c0;

  Ciphertext y = A + prod;
  y.set_output("y");
}
