void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");

  Ciphertext z = x * alpha;            // level 1
  Ciphertext z2 = z * z;               // level 2
  Ciphertext u = z2 + z2 - one;        // T2
  Ciphertext q3 = u + u - one;         // 2u - 1
  Ciphertext c5u = u * c5;             // level 3
  Ciphertext r = c5u + c5u + c3;       // c3 + 2 c5 u
  Ciphertext inner = q3 * r + c1 - c5; // level 4
  Ciphertext result = z * inner + c0;  // level 5
  result.set_output("y");
}
