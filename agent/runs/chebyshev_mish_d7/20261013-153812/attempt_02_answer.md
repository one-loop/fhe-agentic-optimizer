The failure is a level collision. `term3` and `term4` both land at level 4, but they come from different multiplication histories, so CHEHAB cannot add them. The baseline avoided this by giving every term a distinct level.

My fix is to pad each T_k with ct×`one` so that every term reaches the same depth (4) before its coefficient multiply. Every term then has the same multiplication history, and all sums meet at matching levels. The padding costs about 7 cheap plaintext multiplies, which is small next to the 388 ms baseline.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");

  // pad: consume n levels by multiplying with the plaintext 1
  auto pad = [&](Ciphertext c, int n) {
    for (int i = 0; i < n; ++i)
      c = c * one;
    return c;
  };

  // z = x/4 in [-1, 1]  (depth 1)
  Ciphertext z = x * alpha;

  // T2 = 2 z^2 - 1  (depth 2)
  Ciphertext z2 = z * z;
  Ciphertext t2 = z2 + z2 - one;

  // T3 = 2 z T2 - z  (depth 3)
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;

  // T4 = 2 T2^2 - 1  (depth 3)
  Ciphertext t2sq = t2 * t2;
  Ciphertext t4 = t2sq + t2sq - one;

  // T5 = 2 T2 T3 - z  (depth 4)
  Ciphertext t2t3 = t2 * t3;
  Ciphertext t5 = t2t3 + t2t3 - z;

  // T6 = 2 T3^2 - 1  (depth 4)
  Ciphertext t3sq = t3 * t3;
  Ciphertext t6 = t3sq + t3sq - one;

  // T7 = 2 T3 T4 - z  (depth 4)
  Ciphertext t3t4 = t3 * t4;
  Ciphertext t7 = t3t4 + t3t4 - z;

  // equalize depths to 4 so all terms share the same multiplication history
  Ciphertext p1 = pad(z, 3);
  Ciphertext p2 = pad(t2, 2);
  Ciphertext p3 = pad(t3, 1);
  Ciphertext p4 = pad(t4, 1);

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  Ciphertext term1 = p1 * c1;
  Ciphertext term2 = p2 * c2;
  Ciphertext term3 = p3 * c3;
  Ciphertext term4 = p4 * c4;
  Ciphertext term5 = t5 * c5;
  Ciphertext term6 = t6 * c6;
  Ciphertext term7 = t7 * c7;

  Ciphertext result = term1 + term2;
  result = result + term3;
  result = result + term4;
  result = result + term5;
  result = result + term6;
  result = result + term7;
  result = result + c0;

  result.set_output("y");
}
```