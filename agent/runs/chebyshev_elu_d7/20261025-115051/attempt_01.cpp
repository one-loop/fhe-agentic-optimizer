void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2s = square(t2);
  Ciphertext t4 = t2s + t2s - one;
  Ciphertext m5 = t2 * t3;
  Ciphertext t5 = m5 + m5 - z;
  Ciphertext s6 = square(t3);
  Ciphertext t6 = s6 + s6 - one;
  Ciphertext m7 = t3 * t4;
  Ciphertext t7 = m7 + m7 - z;
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
