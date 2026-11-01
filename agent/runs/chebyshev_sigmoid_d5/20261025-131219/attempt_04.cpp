void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Ciphertext z = x * alpha;
  Ciphertext xx = x * x;
  Ciphertext w = xx * alpha;
  Ciphertext zw = z * w;
  Ciphertext zw2 = zw * w;
  Ciphertext z2 = z + z;
  Ciphertext z3 = z2 + z;
  Ciphertext z5 = (z2 + z2) + z;
  Ciphertext zw2x = zw + zw;
  Ciphertext zw5 = (zw2x + zw2x) + zw;
  Ciphertext A = zw - z3;
  Ciphertext B = (zw2 - zw5) + z5;
  Ciphertext y = (z * c1 + A * c3) + B * c5;
  y = y + c0;
  y.set_output("y");
}
