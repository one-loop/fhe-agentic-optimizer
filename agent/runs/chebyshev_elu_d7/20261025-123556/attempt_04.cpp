void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext s = square(z);
  Ciphertext t2 = s + s - one;
  Ciphertext w3 = z * t2;
  Ciphertext t3 = w3 + w3 - z;
  Ciphertext s4 = square(t2);
  Ciphertext t4 = s4 + s4 - one;
  Ciphertext w5 = t2 * t3;
  Ciphertext t5 = w5 + w5 - z;
  Ciphertext s6 = square(t3);
  Ciphertext t6 = s6 + s6 - one;
  Ciphertext w7 = t3 * t4;
  Ciphertext t7 = w7 + w7 - z;

  Ciphertext r1 = z * c1;
  Ciphertext r2 = t2 * c2;
  Ciphertext r3 = t3 * c3;
  Ciphertext r4 = (t4 * c4) * one;
  Ciphertext r5 = (t5 * c5) * one;
  Ciphertext r6 = ((t6 * c6) * one) * one;
  Ciphertext r7 = (((t7 * c7) * one) * one) * one;

  Ciphertext result = r1 + r2;
  result = result + r3;
  result = result + r4;
  result = result + r5;
  result = result + r6;
  result = result + r7;
  result = result + c0;
  result.set_output("y");
}
