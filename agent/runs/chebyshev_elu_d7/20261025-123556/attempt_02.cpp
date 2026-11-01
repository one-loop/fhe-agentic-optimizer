void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext s = square(z);
  Ciphertext t2 = s + s - one;
  Ciphertext w3 = z * t2;
  Ciphertext t3 = w3 + w3 - z;
  Ciphertext s4 = square(t2);
  Ciphertext t4a = s4 + s4 - one;
  Ciphertext w4 = z * t3;
  Ciphertext t4b = w4 + w4 - t2;
  Ciphertext w5 = t2 * t3;
  Ciphertext t5 = w5 + w5 - z;
  Ciphertext s6 = square(t3);
  Ciphertext t6 = s6 + s6 - one;
  Ciphertext w7 = t3 * t4a;
  Ciphertext t7 = w7 + w7 - z;
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result + t3 * c3;
  result = result + t4b * c4;
  result = result + t5 * c5;
  result = result + t6 * c6;
  result = result + t7 * c7;
  result = result + c0;
  result.set_output("y");
}
