Idea: the output is an even/odd mix, y = c0 + c1 z + c2 T2 + c4 T4 + c6 T6. With T2 = 2z²-1, T4 = 2T2²-1 and T6 = 2·T2·T4 - T2, everything can be written as a polynomial in u = z².

Let s = z². Then
T2 = 2s - 1
T4 = 8s² - 8s + 1
T6 = 32s³ - 48s² + 18s - 1

so y = c0 + c1 z + P(s), with P(s) = a1 s + a2 s² + a3 s³ and
- a1 = 2c2 - 8c4 + 18c6 (after combining the constants)
- a2 = 8c4 - 48c6
- a3 = 32c6
- constant = c0 - c2 + c4 - c6

Combining the coefficients would be plaintext-plaintext arithmetic, which is not allowed, so I keep them as ciphertext-plaintext products. Combining coefficients into a new plaintext is impossible, but I can fold them in differently: use the Chebyshev form but with fewer multiplications.

Plan:
- z = x·alpha (a plaintext multiply, level 1).
- s = z·z (a ct-ct multiply, level 2).
- T2 = s + s - one, which needs no multiplication. Subtracting a plaintext `one` costs a plaintext add (~2.5 ms).
- T4 = 2·T2² - one (a ct-ct multiply, level 3).
- T6 = 2·T2·T4 - T2 (a ct-ct multiply, level 4).
- Then multiply by c2, c4, c6, which are plaintext multiplies.

Total: 3 ct-ct multiplies after z, instead of 5. To save depth, I compute c6·T6 as (2·c6·T2)·T4 - c6·T2. Here T2·c6 is at level 3, and T4 is at level 3, so the product lands at level 4. That is the same depth, but it reuses c6·T2 in the other term.

The sum is c2·T2 - c6·T2 + c4·T4 + 2(c6·T2)·T4 + c1·z + c0.
- Terms with T2 are at level 3.
- c4·T4 is at level 4.
- The product (c6T2 + c6T2)·T4 is at level 4.
- c1·z is at level 2.

Scale mismatches should be handled by CHEHAB, since levels differ or the multiplication patterns match.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");
  Ciphertext z = x * alpha;
  Ciphertext s = square(z);
  Ciphertext t2 = s + s - one;
  Ciphertext q = square(t2);
  Ciphertext t4 = q + q - one;
  Ciphertext a = t2 * c6;
  Ciphertext a2 = a + a;
  Ciphertext p = a2 * t4;
  Ciphertext u = t2 * c2;
  Ciphertext v = t4 * c4;
  Ciphertext w = z * c1;
  Ciphertext result = p - a + u + v + w + c0;
  result.set_output("y");
}
```