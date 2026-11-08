This version brings the depth down from 5 to 4 levels by applying every coefficient to z early, at level 2, rather than at the end of the chain. The rewrite uses the identity 2·T_m·T_n = T_{m+n} + T_{|m−n|}:

- c5·T5 + c7·T7 = 2·T4·(c5·z + c7·T3) − c5·T3 − c7·z
- So y = 2·T4·U + (c3 − c5)·T3 + (c1 − c7)·z, with U = c5·z + c7·T3.
- c7·T3 = 2(c7·z)·T2 − c7·z and (c3 − c5)·T3 = 2·d·T2 − d, where d = (c3 − c5)·z.
- Every coefficient product is a scaled z at level 2, so the products with T2 land at level 3 and the final product T4·U at level 4.

Compared with attempt 2, this costs one extra ciphertext multiplication (5 instead of 4). In exchange it drops a level and needs only 2 plaintext additions instead of 5. The plaintext multiplications stay at 5, since c7·z is shared. My rough cost estimate puts it close to attempt 2, so the measurement will decide.

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

  Ciphertext at2 = a * t2;               // L3
  Ciphertext U = e - a + at2 + at2;      // L3  c5 z + c7 T3
  Ciphertext dt2 = d * t2;               // L3

  Ciphertext m = t4 * U;                 // L4
  Ciphertext y = m + m + dt2 + dt2 - d + f;
  y.set_output("y");
}
```