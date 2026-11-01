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
  Ciphertext l1 = z + z;                 // c1*T1 with c1 = 2
  Ciphertext v = z * l1;                 // 2 z^2
  Ciphertext w = v - one;                // T2
  Ciphertext u = square(w);              // T2^2
  Ciphertext a = w * c6;                 // c6*T2
  Ciphertext s = a + a + c4;             // c4 + 2 c6 T2
  Ciphertext p = s + s;
  Ciphertext b = w * c2;                 // c2*T2
  Ciphertext lo = l1 + c0;               // level 1
  Ciphertext q = (b - a - s) + lo;       // level 3
  Ciphertext r4 = u * p;                 // 2 u s
  Ciphertext r = r4 + q;
  r.set_output("y");
}
