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
  Plaintext c7("c7");
  Plaintext c6("c6");

  Ciphertext z = x * alpha;                 // L1
  Ciphertext zz = square(z);                // L2
  Ciphertext T2 = zz + zz - one;            // L2
  Ciphertext T4s = square(T2);              // L3
  Ciphertext T4 = T4s + T4s - one;          // L3

  Ciphertext zc1 = z * c1;                  // L2
  Ciphertext zc3 = z * c3;
  Ciphertext zc5 = z * c5;
  Ciphertext zc7 = z * c7;

  Ciphertext zc7x2 = zc7 + zc7;
  // high part q = (c5-c7) z + T2*(c6 + 2 c7 z)
  Ciphertext r = zc7x2 + c6;                // L2
  Ciphertext h = T2 * r;                    // L3
  Ciphertext q = (zc5 - zc7) + h;           // L3
  Ciphertext P = T4 * (q + q + c4);         // L4

  // low part: (c1-c7-c3+c5) z + T2*((c2-c6) + 2(c3-c5) z)
  Ciphertext s = zc3 - zc5;                 // L2
  Ciphertext u = (s + s + zc7x2 + c2) - r;  // = 2s + c2 - c6
  Ciphertext g = T2 * u;                    // L3
  Ciphertext lin = zc1 - zc7 - s;           // L2

  Ciphertext y = lin + g + P + c0;
  y.set_output("y");
}
