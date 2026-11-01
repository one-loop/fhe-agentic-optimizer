The baseline builds every T_k separately. This kernel uses only the odd Chebyshev terms and folds them into one nested product, so the output needs 4 ciphertext-ciphertext multiplications instead of 6, 2 ciphertext-plaintext multiplications instead of 5, and 6 levels instead of 8.

The identities are:

- T3 = z(2T2 − 1)
- T5 = 2·T2·T3 − T1
- T7 = 2·T3·T4 − T1

These give:

y = c0 + z · [ (c1 − c5 − c7) + (2T2 − 1)·(c3 + 2·inner) ]

where inner = c5·T2 + c7·T4. Since T4 = 2T2² − 1, inner is computed as T2·(2·c7·T2 + c5) − c7.

The coefficient combinations are formed by adding the plaintexts c1, c5 and c7 to ciphertexts, so there is no plaintext-plaintext arithmetic. Terms with different multiplication histories meet at different levels, which avoids scale mismatches.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");

  Ciphertext z = x * alpha;
  Ciphertext zs = z * z;
  Ciphertext t2 = zs + zs - one;          // T2
  Ciphertext u = t2 * c7;                 // c7*T2
  Ciphertext v = u + u + c5;              // 2c7 T2 + c5
  Ciphertext w = t2 * v;                  // 2c7 T2^2 + c5 T2
  Ciphertext inner = w - c7;              // c7 T4 + c5 T2
  Ciphertext m = inner + inner + c3;      // c3 + 2 inner
  Ciphertext q = t2 + t2 - one;           // 2T2 - 1
  Ciphertext qm = q * m;
  Ciphertext s = qm + c1 - c5 - c7;
  Ciphertext r = z * s;
  Ciphertext y = r + c0;
  y.set_output("y");
}
```