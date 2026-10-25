Idea: evaluate the polynomial in the power basis, which needs fewer ciphertext multiplications and fewer plaintext multiplications than the Chebyshev recurrence.

Plaintext inputs are the only source of constants, so I can't precompute the monomial coefficients at compile time. I can still cut work by grouping the polynomial by Chebyshev-style splitting with plaintext products:

- z = x*alpha (1 pt mul).
- z2 = z*z, z4 = z2*z2.
- T2 = 2z2 - 1 (add and subtract only).
- T3 = 2z·T2 - z, T4 = 2T2² - 1.
- Write p = (c0 + c1 T1 + c2 T2 + c3 T3) + T4·(c4 + c5 T1 + c6 T2 + c7 T3), using T4+j relations. Those relations (T_{4+j} = 2 T4 Tj - T_{|4-j|}) are not a plain product, so I'd need extra terms.

Simpler version: the low part L = c0 + c1 z + c2 T2 + c3 T3, and the high part H = c4 T4 + c5 T5 + c6 T6 + c7 T7.
With T_{4+j} = 2 T4 Tj - T_{4-j}:
- T5 = 2 T4 T1 - T3
- T6 = 2 T4 T2 - T2 (since T_{4-2} = T2)
- T7 = 2 T4 T3 - T1

So H = T4·(c4 + 2c5 T1 + 2c6 T2 + 2c7 T3) - c5 T3 - c6 T2 - c7 T1.
The total is then:
- L' = c0 + (c1-c7) T1 + (c2-c6) T2 + (c3-c5) T3
- y = L' + T4·(c4 + 2c5 z + 2c6 T2 + 2c7 T3)

The coefficient combinations are plaintext-plaintext arithmetic, which is not allowed. But I can fold them in as ciphertext products: a coefficient times a ciphertext is allowed, and the differences are applied as subtraction of separate terms.

Terms:
- A = c5·z + c6·T2 + c7·T3 (3 pt muls). Then G = c4 + A + A, using the plaintext add c4.
- y = c0 + c1 z + c2 T2 + c3 T3 - A' + T4·G, where A' = c7 z + c6 T2 + c5 T3. That needs 3 more pt muls.

Cost: ct-ct muls are z2 (z*z), T3 (z*T2), T4 (T2*T2) and T4*G, so 4 instead of 6. Plaintext muls are 1 + 6 + 4 = 11, which is more than 8, but each is cheap. Depth: z (1), T2 (2), T3 (3), G (4), T4*G (5), versus 8 before.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = z * z;
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext z3 = z * t2;
  Ciphertext t3 = z3 + z3 - z;
  Ciphertext q = t2 * t2;
  Ciphertext t4 = q + q - one;
  Ciphertext a = z * c5 + t2 * c6 + t3 * c7;
  Ciphertext g = a + a + c4;
  Ciphertext b = z * c7 + t2 * c6 + t3 * c5;
  Ciphertext low = z * c1 + t2 * c2 + t3 * c3;
  Ciphertext y = t4 * g + low - b + c0;
  y.set_output("y");
}
```