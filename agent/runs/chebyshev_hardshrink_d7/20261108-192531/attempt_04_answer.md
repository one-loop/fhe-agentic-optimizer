The arithmetic is the same as attempt 3. Only the grouping of the additions changes, so the mixed-level terms are aligned in a few combined steps rather than one by one.

Attempt 3 needed 5 scale alignments, at about 5 ms each. Its left-to-right sums brought the level-2 terms d, f and e − a, and the level-3 dt2, up to a higher level one at a time. Here they are grouped first:
- (e − a) is aligned once inside U.
- (f − d) is formed at level 2, joined with 2·dt2 at level 3, and that one sum is aligned to the level-4 product.

This should cut the alignments from 5 to about 3.

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
  Ciphertext t2s = t2 * t2;              // L3
  Ciphertext t4 = t2s + t2s - one;       // L3

  Ciphertext a = z * c7;                 // L2
  Ciphertext e = z * c5;                 // L2
  Ciphertext d = z * c3 - e;             // L2  (c3-c5) z
  Ciphertext f = z * c1 - a;             // L2  (c1-c7) z
  Ciphertext lin = f - d;                // L2
  Ciphertext ea = e - a;                 // L2

  Ciphertext at2 = a * t2;               // L3
  Ciphertext at2x2 = at2 + at2;
  Ciphertext U = at2x2 + ea;             // L3  c5 z + c7 T3
  Ciphertext dt2 = d * t2;               // L3
  Ciphertext dt2x2 = dt2 + dt2;
  Ciphertext S = dt2x2 + lin;            // L3

  Ciphertext m = t4 * U;                 // L4
  Ciphertext mm = m + m;
  Ciphertext y = mm + S;
  y.set_output("y");
}
```