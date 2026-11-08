void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext T2 = z2 + z2 - one;
  Ciphertext a = z * T2;
  Ciphertext T3 = a + a - z;
  Ciphertext b = square(T2);
  Ciphertext T4 = b + b - one;
  Ciphertext c = T2 * T3;
  Ciphertext T5 = c + c - z;
  Ciphertext d = square(T3);
  Ciphertext T6 = d + d - one;
  Ciphertext e = T3 * T4;
  Ciphertext T7 = e + e - z;
  Ciphertext result = z * c1;
  result = result + T2 * c2;
  result = result + T3 * c3;
  result = result + T4 * c4;
  result = result + T5 * c5;
  result = result + T6 * c6;
  result = result + T7 * c7;
  result = result + c0;
  result.set_output("y");
}
