void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext zz = square(z);
  Ciphertext t2 = zz + zz - one;
  Ciphertext p3 = z * t2;
  Ciphertext t3 = p3 + p3 - z;
  Ciphertext p4 = z * t3;
  Ciphertext t4 = p4 + p4 - t2;
  Ciphertext p5 = z * t4;
  Ciphertext t5 = p5 + p5 - t3;
  Ciphertext q6 = square(t3);
  Ciphertext t6 = q6 + q6 - one;
  Ciphertext p7 = t3 * t4;
  Ciphertext t7 = p7 + p7 - z;
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
