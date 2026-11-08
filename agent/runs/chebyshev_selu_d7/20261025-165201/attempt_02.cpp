void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext p3 = z * t2;
  Ciphertext t3 = p3 + p3 - z;
  Ciphertext p4 = square(t2);
  Ciphertext t4 = p4 + p4 - one;
  Ciphertext p5 = t2 * t3;
  Ciphertext t5 = p5 + p5 - z;
  Ciphertext p6 = square(t3);
  Ciphertext t6 = p6 + p6 - one;
  Ciphertext p7 = t3 * t4;
  Ciphertext t7 = p7 + p7 - z;

  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext low = z * c1;
  low = low + t2 * c2;
  Ciphertext r4 = t4 * c4;
  Ciphertext r3 = t3 * c3;
  Ciphertext r6 = t6 * c6;
  Ciphertext r5 = t5 * c5;
  Ciphertext r7 = t7 * c7;
  Ciphertext mid = low + r3;
  Ciphertext top = r5 + r6;
  top = top + r7;
  Ciphertext result = top + r4;
  result = result + mid;
  result = result + c0;
  result.set_output("y");
}
