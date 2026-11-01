void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext u2 = u + u;
  Ciphertext w = u2 + u2;
  Ciphertext zw = z * w;
  Ciphertext zw2 = zw * w;
  Ciphertext z2 = z + z;
  Ciphertext z3 = z2 + z;
  Ciphertext z5 = (z2 + z2) + z;
  Ciphertext zwx2 = zw + zw;
  Ciphertext zw5 = (zwx2 + zwx2) + zw;
  Ciphertext D = zw2 - zw5;
  Ciphertext lin = (z * c1 + z5 * c5) - z3 * c3;
  Ciphertext y = (zw * c3 + D * c5) + lin;
  y = y + c0;
  y.set_output("y");
}
