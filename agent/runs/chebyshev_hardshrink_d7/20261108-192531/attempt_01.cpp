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
  Ciphertext zt2 = z * t2;               // L3
  Ciphertext t3 = zt2 + zt2 - z;         // L3
  Ciphertext t2sq = t2 * t2;             // L3
  Ciphertext t4 = t2sq + t2sq - one;     // L3

  Ciphertext a = t2 * c5;                // L3
  Ciphertext b = t4 * c7;                // L4
  Ciphertext v = a + a + b + b + c3;     // L4
  Ciphertext main_term = t3 * v;         // L5

  Ciphertext lin = z * c1 - z * c5 - z * c7; // L2
  Ciphertext y = main_term + lin;
  y.set_output("y");
}
