void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext a3 = z * t2;
  Ciphertext t3 = a3 + a3 - z;
  Ciphertext t2sq = square(t2);
  Ciphertext t4 = t2sq + t2sq - one;

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  Ciphertext p4 = (t2 * c4) * t2;
  Ciphertext p5 = (t2 * c5) * t3;
  Ciphertext p6 = (t3 * c6) * t3;
  Ciphertext p7 = (t3 * c7) * t4;

  Ciphertext high = p4 + p5;
  high = high + p6;
  high = high + p7;
  high = high + high;
  high = high + t3 * c3;

  Ciphertext low = z * c1;
  low = low + t2 * c2;
  low = low - z * c5;
  low = low - z * c7;

  Ciphertext result = high + low;
  result = result + c0;
  result = result - c4;
  result = result - c6;
  result.set_output("y");
}
