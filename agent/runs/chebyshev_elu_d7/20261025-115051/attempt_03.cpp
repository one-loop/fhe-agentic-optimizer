void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext u = t2 + t2 - one;
  Ciphertext t3 = z * u;
  Ciphertext t2s = square(t2);
  Ciphertext t4 = t2s + t2s - one;
  Ciphertext m5 = t2 * t3;
  Ciphertext t5 = m5 + m5 - z;
  Ciphertext m6 = z * t5;
  Ciphertext t6 = m6 + m6 - t4;
  Ciphertext m7 = z * t6;
  Ciphertext t7 = m7 + m7 - t5;
  Ciphertext r34 = t3 * c3 + t4 * c4;
  Ciphertext r12 = z * c1 + t2 * c2;
  Ciphertext result = r12 + r34;
  result = result + t5 * c5;
  result = result + t6 * c6;
  result = result + t7 * c7;
  result = result + c0;
  result.set_output("y");
}
