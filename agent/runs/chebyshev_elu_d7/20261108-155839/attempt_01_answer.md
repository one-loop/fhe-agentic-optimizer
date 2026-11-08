The kernel uses the product identities T_{a+b} = 2·T_a·T_b − T_{|a−b|} to cut the multiplicative depth from 8 to about 5, with fewer ciphertext-ciphertext multiplications. I have not compiled or run it.

- **Low powers:** z = x·alpha, T2 = 2z² − 1, T3 = 2·z·T2 − z and T4 = 2·T2² − 1.
- **High part:** T5 = 2·T2·T3 − z, T6 = 2·T3² − 1 and T7 = 2·T3·T4 − z, so c3·T3 + c5·T5 + c6·T6 + c7·T7 = T3·(2·(c5·T2 + c6·T3 + c7·T4) + c3) − (c5 + c7)·z − c6. This needs one ciphertext-ciphertext multiplication in place of three.
- **Low part:** c0 + c1·z + c2·T2 + c4·T4, plus the correction terms −c5·z, −c7·z and −c6. The corrections use separate scalar multiplies and a plaintext subtract, since plaintext-plaintext arithmetic is not allowed.
- **Cost:** 4 ciphertext-ciphertext multiplications (z², z·T2, T2², and the final T3 product) instead of 6.

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

  Ciphertext z = x * alpha;
  Ciphertext z2 = z * z;
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext zt = z * t2;
  Ciphertext t3 = zt + zt - z;
  Ciphertext tt = t2 * t2;
  Ciphertext t4 = tt + tt - one;

  Ciphertext u = t2 * c5 + t3 * c6 + t4 * c7;
  Ciphertext inner = u + u + c3;
  Ciphertext hi = t3 * inner;

  Ciphertext low = z * c1 + t2 * c2 + t4 * c4;
  low = low - z * c5;
  low = low - z * c7;
  low = low + c0;
  low = low - c6;

  Ciphertext y = hi + low;
  y.set_output("y");
}
```