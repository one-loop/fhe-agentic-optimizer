// 4 ciphertext-ciphertext multiplications instead of 6, from
// T5 = 2 T4 T1 - T3 and T7 = 2 T4 T3 - T1:
//   c5 T5 + c7 T7 = T4 (2 c5 T1 + 2 c7 T3) - c5 T3 - c7 T1.
// Terms with different multiplication histories meet at different levels.
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c3("c3"), c5("c5"), c7("c7");
  Ciphertext t1 = x * alpha;
  Ciphertext s2 = t1 * t1;
  Ciphertext t2 = s2 + s2 - one;
  Ciphertext p12 = t1 * t2;
  Ciphertext t3 = p12 + p12 - t1;
  Ciphertext s4 = t2 * t2;
  Ciphertext t4 = s4 + s4 - one;

  Ciphertext c5t1 = t1 * c5;
  Ciphertext c7t3 = t3 * c7;
  Ciphertext inner = c5t1 + c5t1 + c7t3 + c7t3;
  Ciphertext high = t4 * inner;

  Ciphertext low1 = t1 * c1 - t1 * c7;
  Ciphertext low3 = t3 * c3 - t3 * c5;
  Ciphertext y = high + low3 + low1 + c0;
  y.set_output("y");
}
