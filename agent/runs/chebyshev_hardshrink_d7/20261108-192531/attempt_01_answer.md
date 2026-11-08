The baseline builds every T_k up to T7 and consumes 8 levels. This version keeps only the four needed coefficients (c1, c3, c5, c7) and regroups the odd terms around T3, using these identities:
- T5 = 2·T2·T3 − z
- T7 = 2·T3·T4 − z

That gives:

y = T3·(c3 + 2c5·T2 + 2c7·T4) + c1·z − c5·z − c7·z

Building T2 = 2z² − 1, T3 = 2z·T2 − z and T4 = 2T2² − 1, plus the final product with T3, takes 4 ciphertext multiplications instead of 6. There are 5 plaintext multiplications, as before. The circuit should consume about 5 levels instead of 8. Sums whose terms were multiplied differently meet at different levels, which should avoid scale mismatches. I have not compiled or run it yet.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");

  Ciphertext z = x * alpha;              // L1
  Ciphertext zz = z * z;                 // L2
  Ciphertext t2 = zz + zz - one;         // L2
  Ciphertext zt2 = z * t2;               // L3
  Ciphertext t3 = zt2 + zt2 - z;         // L3
  Ciphertext t2sq = t2 * t2;             // L3
  Ciphertext t4 = t2sq + t2sq - one;     // L3

  Ciphertext a = t2 * c5;                // L3
  Ciphertext b = t4 * c7;                // L4
  Ciphertext v = a + a + b + b + c3;     // L4
  Ciphertext main_term = t3 * v;         // L5

  Ciphertext lin = z * c1 - z * c5 - z * c7; // L2
  Ciphertext y = main_term + lin;
  y.set_output("y");
}
```