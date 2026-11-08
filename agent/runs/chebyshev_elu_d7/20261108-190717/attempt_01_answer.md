The idea is to split the polynomial around T4 so it needs fewer multiplications and fewer levels.

**Rewrite.** I use the identity T_{4+j} = 2·T4·T_j − T_{4−j}:
- T5 = 2·T4·T1 − T3
- T6 = 2·T4·T2 − T2
- T7 = 2·T4·T3 − T1

Substituting these gives

y = [c0 + (c1−c7)·T1 + (c2−c6)·T2 + (c3−c5)·T3] + T4·[c4 + 2c5·T1 + 2c6·T2 + 2c7·T3]

**Ciphertext multiplications.** Only T1..T4 are needed:
- T2 = 2z² − 1
- T3 = 2z·T2 − z
- T4 = 2T2² − 1

These take 3 multiplications, plus 1 for the final T4·q, so 4 in total instead of 6.

**Plaintext multiplications.** The c6·T2 product is shared between the two brackets. That leaves 8 ct×pt multiplications, the same count as the baseline.

**Depth.** The multiplicative depth falls from 8 to 5 levels:
- z is at level 1, T2 at level 2, and T3 and T4 at level 3.
- Both brackets reach level 4, and the result is at level 5.

**Scale alignment.** Terms that meet at the same level have the same multiplication history, for example c1·T1 and c7·T1. Everything else meets at different levels, which CHEHAB aligns for free.

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

  Ciphertext z = x * alpha;            // T1, L1
  Ciphertext zz = z * z;
  Ciphertext t2 = zz + zz - one;       // L2
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;       // L3
  Ciphertext t2s = t2 * t2;
  Ciphertext t4 = t2s + t2s - one;     // L3

  // high part q = c4 + 2c5 T1 + 2c6 T2 + 2c7 T3
  Ciphertext a5 = z * c5;              // L2
  Ciphertext a6 = t2 * c6;             // L3
  Ciphertext a7 = t3 * c7;             // L4
  Ciphertext h = a5 + a6 + a7;
  Ciphertext q = h + h + c4;

  // low part: c0 + (c1-c7)T1 + (c2-c6)T2 + (c3-c5)T3
  Ciphertext l1 = z * c1 - z * c7;     // L2
  Ciphertext l2 = t2 * c2 - a6;        // L3
  Ciphertext l3 = t3 * c3 - t3 * c5;   // L4
  Ciphertext low = l1 + l2 + l3 + c0;

  Ciphertext y = t4 * q + low;         // L5
  y.set_output("y");
}
```