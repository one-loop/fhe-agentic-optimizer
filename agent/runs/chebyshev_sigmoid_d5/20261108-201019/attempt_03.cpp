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
  Ciphertext s = x * c5;               // level 1  (= 4 c5 z)
  Ciphertext z2 = z * z;               // level 2
  Ciphertext u = z2 + z2 - one;        // T2
  Ciphertext q3 = u + u - one;         // 4z^2 - 3
  Ciphertext t = s * z;                // level 2, = 4 c5 z^2 = c5 (q3 + 3)
  Ciphertext r = t + c3 - c5 - c5;     // c5 q3 + c3 + c5
  Ciphertext inner = q3 * r + c1 - c5; // level 3
  Ciphertext result = z * inner + c0;  // level 4
  result.set_output("y");
}
