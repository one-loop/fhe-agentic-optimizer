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
  Ciphertext zz = z * z;               // level 2
  Ciphertext t2 = zz + zz - one;       // level 2
  Ciphertext t2s = t2 * t2;            // level 3
  Ciphertext w4 = t2s + t2s;           // level 3, = T4 + 1

  Ciphertext u = t2 * c6;              // level 3
  Ciphertext u2 = u + u;
  Ciphertext inner = u2 + c4;          // level 3
  Ciphertext prod = w4 * inner;        // level 4

  Ciphertext term2 = t2 * c2;          // level 3
  Ciphertext mid = term2 - u2 - u - inner + u2; // = c2*T2 - 3u - c4 + ... simplified below
  // mid = term2 - u - inner  (u2 terms cancel)
  Ciphertext term1 = z * c1;           // level 2
  Ciphertext low = (term2 - u - inner) + term1; // level 3

  Ciphertext result = prod + low + c0;
  result.set_output("y");
}
