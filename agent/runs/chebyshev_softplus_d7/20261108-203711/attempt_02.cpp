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
  Ciphertext t4 = t2s + t2s - one;     // level 3

  Ciphertext u = t2 * c6;              // level 3
  Ciphertext inner = u + u + c4;       // level 3
  Ciphertext prod = t4 * inner;        // level 4

  Ciphertext term2 = t2 * c2;          // level 3
  Ciphertext mid = term2 - u;          // level 3, same scale pattern
  Ciphertext term1 = z * c1;           // level 2
  Ciphertext low = mid + term1;        // level 3

  Ciphertext result = prod + low + c0;
  result.set_output("y");
}
