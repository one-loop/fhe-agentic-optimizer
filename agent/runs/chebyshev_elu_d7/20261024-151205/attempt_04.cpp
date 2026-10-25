void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext zc7 = z * c7;
  Ciphertext b6 = (zc7 + zc7) + c6;
  Ciphertext w5 = z * b6;
  Ciphertext b5 = ((w5 + w5) + c5) - c7;
  Ciphertext w4 = z * b5;
  Ciphertext b4 = ((w4 + w4) + c4) - b6;
  Ciphertext w3 = z * b4;
  Ciphertext b3 = ((w3 + w3) + c3) - b5;
  Ciphertext w2 = z * b3;
  Ciphertext b2 = ((w2 + w2) + c2) - b4;
  Ciphertext w1 = z * b2;
  Ciphertext b1 = ((w1 + w1) + c1) - b3;
  Ciphertext w0 = z * b1;
  Ciphertext y = (w0 + c0) - b2;
  y.set_output("y");
}
