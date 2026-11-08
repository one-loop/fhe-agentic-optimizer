void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext p3 = z * t2;
  Ciphertext t3 = p3 + p3 - z;
  Ciphertext p4 = square(t2);
  Ciphertext t4 = p4 + p4 - one;

  Ciphertext r1 = z * c1;
  Ciphertext r2 = t2 * c2;
  Ciphertext zc3 = z * c3;
  Ciphertext q3 = zc3 * t2;
  Ciphertext r3 = q3 + q3 - zc3;

  Ciphertext t2c4 = t2 * c4;
  Ciphertext q4 = t2c4 * t2;
  Ciphertext r4 = q4 + q4 - c4;

  Ciphertext t2c5 = t2 * c5;
  Ciphertext q5 = t2c5 * t3;
  Ciphertext r5 = q5 + q5 - z * c5;

  Ciphertext t3c6 = t3 * c6;
  Ciphertext q6 = t3c6 * t3;
  Ciphertext r6 = q6 + q6 - c6;

  Ciphertext t3c7 = t3 * c7;
  Ciphertext q7 = t3c7 * t4;
  Ciphertext r7 = q7 + q7 - z * c7;

  Ciphertext result = r1 + r2;
  result = result + r3;
  result = result + r4;
  result = result + r5;
  result = result + r6;
  result = result + r7;
  result = result + c0;
  result.set_output("y");
}
