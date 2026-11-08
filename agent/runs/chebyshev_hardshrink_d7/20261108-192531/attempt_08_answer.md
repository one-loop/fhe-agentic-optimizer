This is the last attempt, and I don't expect it to beat attempt 4. It uses the same 5 ciphertext and 5 plaintext multiplications at 4 levels.

The change folds the linear corrections into the multiplicand w' = 2·T2 − 1:
- U = a·w' + e
- S = d·w' + f
- y = 4·T2²·U − 2U + S, using the 2·T4 = 4·T2² − 2 rewrite from attempt 5

This removes the separate lin and ea subtractions. The cost is one extra plaintext addition of `one` to form w', so any change in run time will be small.

The remaining overhead is the 3 scale alignments. Removing them seems to need either an extra ciphertext multiplication or an extra level, both of which cost more than the roughly 5 ms each alignment takes.

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
  Ciphertext wp = t2 + t2 - one;         // L2  2T2 - 1
  Ciphertext t2s = t2 * t2;              // L3  T2^2

  Ciphertext a = z * c7;                 // L2
  Ciphertext e = z * c5;                 // L2
  Ciphertext d = z * c3 - e;             // L2  (c3-c5) z
  Ciphertext f = z * c1 - a;             // L2  (c1-c7) z

  Ciphertext aw = a * wp;                // L3  c7 T3
  Ciphertext U = aw + e;                 // L3  c5 z + c7 T3
  Ciphertext dw = d * wp;                // L3  (c3-c5) T3
  Ciphertext U2 = U + U;
  Ciphertext S0 = dw - U2;               // L3
  Ciphertext S = S0 + f;                 // L3

  Ciphertext m = t2s * U;                // L4
  Ciphertext m2 = m + m;
  Ciphertext m4 = m2 + m2;
  Ciphertext y = m4 + S;
  y.set_output("y");
}
```