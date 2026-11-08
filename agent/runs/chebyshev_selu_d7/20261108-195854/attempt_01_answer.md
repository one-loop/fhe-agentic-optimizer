This evaluates the polynomial with a Paterson–Stockmeyer style split: the baby steps are T1 and T2, and the giant step is T4 = 2T2² − 1. I used the identity T_{4+j} = 2·T4·T_j − T_{4−j}. With it the output becomes
y = c0 + (c1−c7)T1 + (c2−c6)T2 + (c3−c5)T3 + T4·(2q + c4), where q = c5T1 + c6T2 + c7T3.

Each T3 term is expanded as T3 = 2zT2 − z, so the coefficient multiplies z before the ciphertext multiply. As a result:
- **q** is (c5−c7)z + T2·(c6 + 2c7z).
- **Low-order part** is (c1−c7−c3+c5)z + T2·((c2−c6) + 2(c3−c5)z).

Expected circuit compared with the baseline:

| | This kernel | Baseline |
|---|---|---|
| Levels consumed | 4 | 8 |
| Ciphertext × ciphertext multiplies | 5 | 6 |
| Ciphertext × plaintext multiplies | 5 (alpha, c1, c3, c5, c7) | 8 |

Terms with different multiplication histories meet only at different levels, which should avoid scale mismatches.

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

  Ciphertext z = x * alpha;                 // L1
  Ciphertext zz = z * z;                    // L2
  Ciphertext T2 = zz + zz - one;            // L2
  Ciphertext T4s = T2 * T2;                 // L3
  Ciphertext T4 = T4s + T4s - one;          // L3

  Ciphertext zc1 = z * c1;                  // L2
  Ciphertext zc3 = z * c3;
  Ciphertext zc5 = z * c5;
  Ciphertext zc7 = z * c7;

  // high part q = (c5-c7) z + T2*(c6 + 2 c7 z)
  Ciphertext r = zc7 + zc7 + c6;            // L2
  Ciphertext h = T2 * r;                    // L3
  Ciphertext q = (zc5 - zc7) + h;           // L3
  Ciphertext P = T4 * (q + q + c4);         // L4

  // low part: (c1-c7-c3+c5) z + T2*((c2-c6) + 2(c3-c5) z)
  Ciphertext s = zc3 - zc5;                 // L2
  Ciphertext u = s + s + c2 - c6;           // L2
  Ciphertext g = T2 * u;                    // L3
  Ciphertext lin = zc1 - zc7 - zc3 + zc5;   // L2

  Ciphertext y = lin + g + P + c0;
  y.set_output("y");
}
```