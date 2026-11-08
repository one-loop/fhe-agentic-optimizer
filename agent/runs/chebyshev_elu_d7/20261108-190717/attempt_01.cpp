void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  Ciphertext z = x * alpha;            // T1, L1
  Ciphertext zz = z * z;
  Ciphertext t2 = zz + zz - one;       // L2
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;       // L3
  Ciphertext t2s = t2 * t2;
  Ciphertext t4 = t2s + t2s - one;     // L3

  // high part q = c4 + 2c5 T1 + 2c6 T2 + 2c7 T3
  Ciphertext a5 = z * c5;              // L2
  Ciphertext a6 = t2 * c6;             // L3
  Ciphertext a7 = t3 * c7;             // L4
  Ciphertext h = a5 + a6 + a7;
  Ciphertext q = h + h + c4;

  // low part: c0 + (c1-c7)T1 + (c2-c6)T2 + (c3-c5)T3
  Ciphertext l1 = z * c1 - z * c7;     // L2
  Ciphertext l2 = t2 * c2 - a6;        // L3
  Ciphertext l3 = t3 * c3 - t3 * c5;   // L4
  Ciphertext low = l1 + l2 + l3 + c0;

  Ciphertext y = t4 * q + low;         // L5
  y.set_output("y");
}
