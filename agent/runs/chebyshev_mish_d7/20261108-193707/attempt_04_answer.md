**Change:** I drop the plaintext subtraction `- one` from T4. Since T4 = 2·T2² − 1, the giant-step product becomes

T4·a2 = (2·T2²)·a2 − a2.

The extra −a2 sits at level 4, the same level as the correction sum s, so I merge it there. This removes one plaintext add and costs one ciphertext add. Everything else is unchanged: depth 5, 4 ct-ct multiplications, 9 ct-pt multiplications.

The expected gain is small, roughly 1–2 ms, so it may not clear the 3% bar. I looked for bigger wins and found none that work:

- **Depth 4:** c6·T2 and c7·T3 always land at level 3 with different scales, which causes the same mismatch as in attempt 1.
- **Fewer coefficient multiplications:** these need either plaintext-plaintext arithmetic, which is not allowed, or an extra ct-ct multiplication, which costs more than it saves.

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
  Ciphertext t44 = t22 + t22;                // T4 + 1, level 3

  // A = c5 T1 + c6 T2 + c7 T3
  Ciphertext c6t2 = t2 * c6;
  Ciphertext a = t1 * c5 + (c6t2 + t3 * c7);
  Ciphertext a2 = a + a + c4;                // level 4
  Ciphertext w = t44 * a2;                   // level 5

  // low - B grouped by level, minus a2 (from T4 = t44 - 1)
  Ciphertext s2 = t1 * c1 - t1 * c7;         // level 2
  Ciphertext s3 = t2 * c2 - c6t2;            // level 3
  Ciphertext s4 = t3 * c3 - t3 * c5;         // level 4
  Ciphertext s = s2 + (s3 + (s4 - a2));

  Ciphertext result = w + s + c0;
  result.set_output("y");
}
```