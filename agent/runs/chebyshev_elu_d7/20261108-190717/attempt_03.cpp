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

  Ciphertext z = x * alpha;            // L1
  Ciphertext zz = z * z;
  Ciphertext t2 = zz + zz - one;       // L2
  Ciphertext p = t2 * t2;              // L3, T4 = 2p - 1

  Ciphertext a1 = z * c1;              // L2
  Ciphertext a3 = z * c3;
  Ciphertext a5 = z * c5;
  Ciphertext a7 = z * c7;

  // q = c4 + 2(c5-c7) z + 2 T2 (c6 + 2 c7 z)
  Ciphertext v = a7 + a7 + c6;         // L2
  Ciphertext hv = v * t2;              // L3
  Ciphertext d57 = a5 - a7;
  Ciphertext s = d57 + d57 + c4;       // L2
  Ciphertext q = hv + hv + s;          // L3

  // low = r + T2 m, g = (c3-c5) z
  Ciphertext g = a3 - a5;
  Ciphertext m = g + g + c2 - c6;      // L2
  Ciphertext lm = m * t2;              // L3

  // r - s = (c1 + c7 - c3 - c5) z + c0 - c4
  Ciphertext rs = a1 + a7 - a3 - a5 + c0 - c4;   // L2

  Ciphertext pq = p * q;               // L4
  Ciphertext y = pq + pq + ((lm - hv - hv) + rs);
  y.set_output("y");
}
