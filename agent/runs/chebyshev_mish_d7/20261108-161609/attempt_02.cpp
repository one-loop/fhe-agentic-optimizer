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
  Ciphertext u7 = t3 * c7;
  Ciphertext B = (u5 + u6 + c4);
  B = B + B;
  B = B + (u7 + u7);

  Ciphertext prod = t4 * B;

  Ciphertext a1 = z * c1;
  Ciphertext a7 = z * c7;
  Ciphertext a2 = t2 * c2;
  Ciphertext a3 = t3 * c3;
  Ciphertext a5 = t3 * c5;
  Ciphertext lo = (a1 - a7) + c0;
  Ciphertext mid = a2 - u6;
  Ciphertext hi = a3 - a5;
  Ciphertext A = (lo + mid) + hi;

  Ciphertext y = A + prod;
  y.set_output("y");
}
