void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext z2 = z * z;
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext m3 = z * t2;
  Ciphertext t3 = m3 + m3 - z;
  Ciphertext m4 = z * t3;
  Ciphertext t4 = m4 + m4 - t2;
  Ciphertext m5 = z * t4;
  Ciphertext t5 = m5 + m5 - t3;
  Ciphertext m6 = t3 * t3;
  Ciphertext t6 = m6 + m6 - one;
  Ciphertext m7 = t3 * t4;
  Ciphertext t7 = m7 + m7 - z;
  Plaintext c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7"), c0("c0");
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result + t3 * c3;
  result = result + t4 * c4;
  result = result + t5 * c5;
  result = result + t6 * c6;
  result = result + t7 * c7;
  result = result + c0;
  result.set_output("y");
}
