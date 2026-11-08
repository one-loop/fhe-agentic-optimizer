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
  Ciphertext s6 = square(t3);
  Ciphertext t6 = s6 + s6 - one;
  Ciphertext m7 = t3 * t4;
  Ciphertext t7 = m7 + m7 - z;
  Ciphertext r34 = t3 * c3 + t4 * c4;
  Ciphertext r56 = t5 * c5 + t6 * c6;
  Ciphertext r7 = t7 * c7;
  Ciphertext r12 = z * c1 + t2 * c2;
  Ciphertext result = r12 + r34;
  result = result + r56;
  result = result + r7;
  result = result + c0;
  result.set_output("y");
}
