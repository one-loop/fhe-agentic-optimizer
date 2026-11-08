**Two changes to cut CHEHAB's scale alignments (match_scale), each costing about 5 ms:**

1. **T3 as a product.** I compute T3 = z·(2·T2 − 1) instead of 2·z·T2 − z. This removes the alignment of the level-1 z into a level-3 sum. The ct-ct multiplication count stays at 4.
2. **Sums grouped by level.** I group the correction sum `low − b` by level: (c1 − c7)·T1 at level 2, (c2 − c6)·T2 at level 3 and (c3 − c5)·T3 at level 4. Each combination of terms at the same level happens before any cross-level add, so fewer alignments should be needed.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  Ciphertext t1 = x * alpha;                 // level 1
  Ciphertext zz = t1 * t1;
  Ciphertext t2 = zz + zz - one;             // level 2
  Ciphertext u = t2 + t2 - one;              // 2*T2 - 1, level 2
  Ciphertext t3 = t1 * u;                    // level 3
  Ciphertext t22 = t2 * t2;
  Ciphertext t4 = t22 + t22 - one;           // level 3

  // A = c5 T1 + c6 T2 + c7 T3
  Ciphertext c6t2 = t2 * c6;
  Ciphertext a = t1 * c5 + (c6t2 + t3 * c7);
  Ciphertext a2 = a + a + c4;
  Ciphertext w = t4 * a2;                    // level 5

  // low - B grouped by level
  Ciphertext s2 = t1 * c1 - t1 * c7;         // level 2
  Ciphertext s3 = t2 * c2 - c6t2;            // level 3
  Ciphertext s4 = t3 * c3 - t3 * c5;         // level 4
  Ciphertext s = s2 + (s3 + s4);

  Ciphertext result = w + s + c0;
  result.set_output("y");
}
```