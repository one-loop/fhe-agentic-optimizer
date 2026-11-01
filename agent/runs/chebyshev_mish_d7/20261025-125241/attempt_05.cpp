void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2s = square(t2);
  Ciphertext t4 = t2s + t2s - one;
  Ciphertext z4 = z * t4;
  Ciphertext t5 = z4 + z4 - t3;
  Ciphertext z5 = z * t5;
  Ciphertext t6 = z5 + z5 - t4;
  Ciphertext z6 = z * t6;
  Ciphertext t7 = z6 + z6 - t5;

  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result + t4 * c4;
  result = result + t5 * c5;
  result = result + t6 * c6;
  result = result + t7 * c7;
  result = result + t3 * c3;
  result = result + c0;
  result.set_output("y");
}
