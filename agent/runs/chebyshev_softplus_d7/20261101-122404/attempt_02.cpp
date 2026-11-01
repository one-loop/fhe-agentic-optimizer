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
  Ciphertext z2 = z * z;
  Ciphertext w = z2 + z2 - one;          // T2
  Ciphertext u = w * w;                  // T2^2
  Ciphertext a = w * c6;                 // c6*T2
  Ciphertext s = a + a + c4;             // c4 + 2 c6 T2
  Ciphertext p = s + s;
  Ciphertext b = w * c2;                 // c2*T2
  Ciphertext q = b - a - s;              // level 3
  Ciphertext l1 = z * c1;                // level 2
  Ciphertext low = q + l1 + c0;
  Ciphertext r4 = u * p;                 // 2 u s
  Ciphertext r = r4 + low;
  r.set_output("y");
}
