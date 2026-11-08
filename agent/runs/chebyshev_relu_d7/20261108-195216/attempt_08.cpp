void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");

  Ciphertext z = x * alpha;
  Ciphertext zz = square(z);
  Ciphertext w = zz + zz - one;          // T2
  Ciphertext ww = square(w);
  Ciphertext t4 = ww + ww - one;         // T4
  Ciphertext u = w * c6;                 // c6*T2
  Ciphertext inner = u + u + c4;         // 2 c6 T2 + c4
  Ciphertext hi = t4 * inner;            // c4 T4 + c6 T6 + c6 T2
  Ciphertext low = w * c2 - u;           // (c2 - c6) T2
  Ciphertext lin = z * c1;
  Ciphertext result = hi + low + lin + c0;
  result.set_output("y");
}
