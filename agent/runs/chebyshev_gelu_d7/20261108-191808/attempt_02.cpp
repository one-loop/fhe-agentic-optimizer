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
  Ciphertext lin = z * c1 + c0;        // level 2
  Ciphertext zz = z * z;               // level 2
  Ciphertext u = zz + zz - one;        // T2, level 2
  Ciphertext c6u = u * c6;             // level 3
  Ciphertext mid = u * c2 - c6u;       // (c2 - c6) u, level 3
  Ciphertext low = mid + lin;          // level 3
  Ciphertext uu = u * u;               // level 3
  Ciphertext t4 = uu + uu - one;       // T4, level 3
  Ciphertext w = c6u + c6u + c4;       // c4 + 2 c6 u, level 3
  Ciphertext high = t4 * w;            // level 4
  Ciphertext result = high + low;
  result.set_output("y");
}
