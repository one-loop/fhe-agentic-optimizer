void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Ciphertext z = x * alpha;
  Ciphertext s = square(z);
  Ciphertext d = s * c5;
  Ciphertext f = s * c3;
  Ciphertext e = d * s;
  // 16e - 20d + 4f = 4*(4e - 5d + f)
  Ciphertext e2 = e + e;
  Ciphertext e4 = e2 + e2;
  Ciphertext d2 = d + d;
  Ciphertext d4 = d2 + d2;
  Ciphertext d5 = d4 + d;
  Ciphertext inner = e4 - d5 + f;
  Ciphertext i2 = inner + inner;
  Ciphertext P = i2 + i2;
  P = P + c1;
  P = P - c3;
  P = P - c3;
  P = P - c3;
  P = P + c5;
  P = P + c5;
  P = P + c5;
  P = P + c5;
  P = P + c5;
  Ciphertext y = z * P;
  y = y + c0;
  y.set_output("y");
}
