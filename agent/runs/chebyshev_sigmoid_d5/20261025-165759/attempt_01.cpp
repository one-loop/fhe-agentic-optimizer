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
  // 16 e
  Ciphertext e2 = e + e;
  Ciphertext e4 = e2 + e2;
  Ciphertext e8 = e4 + e4;
  Ciphertext e16 = e8 + e8;
  // 4f - 20d = 4f - 16d - 4d
  Ciphertext d2 = d + d;
  Ciphertext d4 = d2 + d2;
  Ciphertext d8 = d4 + d4;
  Ciphertext d16 = d8 + d8;
  Ciphertext f2 = f + f;
  Ciphertext f4 = f2 + f2;
  Ciphertext lin = f4 - d16 - d4;
  Ciphertext P = e16 + lin;
  // constant: c1 - 3c3 + 5c5
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
