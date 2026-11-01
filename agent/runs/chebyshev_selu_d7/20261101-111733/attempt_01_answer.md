The baseline builds T2 through T7 one after another, which takes 6 ciphertext multiplications and 8 levels. This kernel uses a baby-step/giant-step Chebyshev split instead: baby steps T1, T2, T3 and one giant step T4.

The identity T_{4+j} = 2·T4·T_j − T_{4−j} gives:

y = c0 + (c1T1 − c7T1) + (c2T2 − c6T2) + (c3T3 − c5T3) + T4·(2q + c4), where q = c5T1 + c6T2 + c7T3.

- **Ciphertext multiplications drop from 6 to 4:** z², z·T2, T2² and T4·(2q + c4).
- **Depth drops from 8 to about 5.**
- **Plaintext multiplications:** c6T2 is reused, so there are 9 in total, all with scalars.
- **Scales:** terms added at the same level share the same multiplication pattern (c3T3 with c5T3, c2T2 with c6T2). Other terms meet at different levels, which CHEHAB aligns.

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

  Ciphertext T1 = x * alpha;
  Ciphertext zz = T1 * T1;
  Ciphertext T2 = zz + zz - one;
  Ciphertext zt = T1 * T2;
  Ciphertext T3 = zt + zt - T1;
  Ciphertext t22 = T2 * T2;
  Ciphertext T4 = t22 + t22 - one;

  Ciphertext a5 = T1 * c5;
  Ciphertext a6 = T2 * c6;
  Ciphertext a7 = T3 * c7;
  Ciphertext q = a7 + a6 + a5;
  Ciphertext inner = q + q + c4;
  Ciphertext high = T4 * inner;

  Ciphertext l1 = T1 * c1 - T1 * c7;
  Ciphertext l2 = T2 * c2 - a6;
  Ciphertext l3 = T3 * c3 - T3 * c5;
  Ciphertext low = l3 + l2 + l1;

  Ciphertext result = high + low + c0;
  result.set_output("y");
}
```