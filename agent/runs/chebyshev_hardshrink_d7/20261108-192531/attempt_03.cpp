void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");

  Ciphertext z = x * alpha;              // L1
  Ciphertext zz = z * z;                 // L2
  Ciphertext t2 = zz + zz - one;         // L2
  Ciphertext t2s = t2 * t2;              // L3
  Ciphertext t4 = t2s + t2s - one;       // L3

  Ciphertext a = z * c7;                 // L2
  Ciphertext e = z * c5;                 // L2
  Ciphertext d = z * c3 - e;             // L2  (c3-c5) z
  Ciphertext f = z * c1 - a;             // L2  (c1-c7) z

  Ciphertext at2 = a * t2;               // L3
  Ciphertext U = e - a + at2 + at2;      // L3  c5 z + c7 T3
  Ciphertext dt2 = d * t2;               // L3

  Ciphertext m = t4 * U;                 // L4
  Ciphertext y = m + m + dt2 + dt2 - d + f;
  y.set_output("y");
}
