Attempt 1 remains the best. Its remaining overhead is mostly plaintext additions, so this version cuts them from 9 to 6. Multiplications stay the same: 4 ciphertext-ciphertext, 2 ciphertext-plaintext, 6 levels.

The idea is to use the other Chebyshev recurrences:

- T7 = 2·T2·T5 − T3
- T5 = 2·T2·T3 − T1

Substituting them gives:

y = c0 + T1·(c1 − g) + T3·(c3 − c7 + 2·T2·g), with g = c5 + 2c7·T2.

With T3 = z·(2T2 − 1), this becomes y = c0 + z·[c1 − g + (2T2 − 1)·n], where n = 2·T2·g + c3 − c7. The product (2T2 − 1)·n is evaluated as 2·T2·n − n, which removes the `one` addition that q needed.

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
  Ciphertext u = t2 * c7;                 // c7 T2
  Ciphertext g = u + u + c5;              // c5 + 2 c7 T2
  Ciphertext h = t2 * g;
  Ciphertext n = h + h + c3 - c7;         // c3 - c7 + 2 T2 g
  Ciphertext tn = t2 * n;
  Ciphertext s = tn + tn - n - g + c1;    // c1 - g + (2T2-1) n
  Ciphertext r = z * s;
  Ciphertext y = r + c0;
  y.set_output("y");
}
```