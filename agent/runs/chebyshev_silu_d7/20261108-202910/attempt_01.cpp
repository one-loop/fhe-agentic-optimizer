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

  Ciphertext z = x * alpha;            // level 1
  Ciphertext u = z * z;                // level 2
  Ciphertext t2 = u + u - one;         // level 2
  Ciphertext s = t2 * t2;              // level 3
  Ciphertext t4 = s + s - one;         // level 3

  Ciphertext c6t2 = t2 * c6;           // level 3
  Ciphertext w = c6t2 + c6t2 + c4;     // level 3
  Ciphertext high = t4 * w;            // level 4: c4*T4 + c6*T6 + c6*T2

  Ciphertext c2t2 = t2 * c2;           // level 3
  Ciphertext mid = c2t2 - c6t2;        // level 3

  Ciphertext lin = z * c1;             // level 2

  Ciphertext result = high + mid + lin + c0;
  result.set_output("y");
}
