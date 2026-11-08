This rewrites attempt 5 in terms of W = 2·T2, which removes most of the doubling additions:

- U = a·W + (e − a) = c5·z + c7·T3
- y = (W·W)·U − 2U + d·W + lin

The multiplication counts are the same: 5 ciphertext, 5 plaintext, 4 levels. Only a few ciphertext additions go away, so I expect the gain to be small and it may not clear the 3% bar. The main remaining overhead is the 3 scale alignments, and I could not find a regrouping that removes them without adding a multiplication or a level.

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
  Ciphertext w = t2 + t2;                // L2  2 T2
  Ciphertext ww = w * w;                 // L3  4 T2^2

  Ciphertext a = z * c7;                 // L2
  Ciphertext e = z * c5;                 // L2
  Ciphertext d = z * c3 - e;             // L2  (c3-c5) z
  Ciphertext f = z * c1 - a;             // L2  (c1-c7) z
  Ciphertext lin = f - d;                // L2
  Ciphertext ea = e - a;                 // L2

  Ciphertext aw = a * w;                 // L3
  Ciphertext U = aw + ea;                // L3  c5 z + c7 T3
  Ciphertext dw = d * w;                 // L3
  Ciphertext U2 = U + U;
  Ciphertext S0 = dw - U2;               // L3
  Ciphertext S = S0 + lin;               // L3

  Ciphertext m = ww * U;                 // L4
  Ciphertext y = m + S;
  y.set_output("y");
}
```