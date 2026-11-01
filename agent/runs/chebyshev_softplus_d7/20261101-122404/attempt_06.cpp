void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");

  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext w = z2 + z2 - one;          // T2
  Ciphertext u = square(w);
  Ciphertext t4 = u + u - one;           // T4
  Ciphertext a = w * c6;                 // c6*T2
  Ciphertext s = a + a + c4;             // c4 + 2 c6 T2
  Ciphertext b = w * c2;                 // c2*T2
  Ciphertext low = b - a + c0;           // level 3
  Ciphertext r4 = t4 * s;                // c4 T4 + c6 (T6 + T2)
  Ciphertext l1 = z + z;                 // c1*T1 with c1 = 2
  Ciphertext r = (r4 + low) + l1;
  r.set_output("y");
}
