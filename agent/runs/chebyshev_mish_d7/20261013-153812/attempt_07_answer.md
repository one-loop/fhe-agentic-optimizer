Attempt 6 used the same kernel as attempt 4 and measured 339 ms against 334.5 ms. That gap is run-to-run noise, so the attempt 4 kernel stays the best. I'm not spending more attempts on the remaining ideas, since the plaintext-multiplication count is already close to its floor for this decomposition and the expected gains are small.

The kernel uses T2, T3, T4 with three ciphertext multiplications, then one product T4·B, for four in total at depth 5. It rewrites the degree-7 sum as c0 + (c1−c7)T1 + (c2−c6)T2 + (c3−c5)T3 + T4·(2c5T1 + 2c6T2 + 2c7T3 + c4), using T4·T_m = (T_{m+4} + T_{4−m})/2.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");

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

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  // inner B = 2c5 T1 + 2c6 T2 + 2c7 T3 + c4   (depth 4)
  Ciphertext z1 = z * c5;
  Ciphertext z1d = z1 + z1;            // level 2
  Ciphertext z2c = t2 * c6;
  Ciphertext z2d = z2c + z2c;          // level 3
  Ciphertext z3 = t3 * c7;
  Ciphertext z3d = z3 + z3;            // level 4
  Ciphertext B = z1d + z2d;            // levels 2,3 -> 3
  B = B + z3d;                         // levels 3,4 -> 4
  B = B + c4;                          // plaintext add

  // product T4 * B  (depth 5)
  Ciphertext prod = t4 * B;

  // linear part: (c1-c7) T1 + (c2-c6) T2 + (c3-c5) T3 + c0
  Ciphertext P1 = z * c1;
  Ciphertext Q1 = z * c7;
  Ciphertext P2 = t2 * c2;
  Ciphertext Q2 = t2 * c6;
  Ciphertext P3 = t3 * c3;
  Ciphertext Q3 = t3 * c5;
  Ciphertext D1 = P1 - Q1;             // level 2
  Ciphertext D2 = P2 - Q2;             // level 3
  Ciphertext D3 = P3 - Q3;             // level 4
  Ciphertext O = D1 + D2;              // levels 2,3 -> 3
  O = O + D3;                          // levels 3,4 -> 4
  O = O + c0;                          // plaintext add

  Ciphertext result = prod + O;        // levels 5,4 -> 5
  result.set_output("y");
}
```