Idea: write the odd polynomial in z as y = c0 + z·(c1 + c3·(4z² − 3) + c5·(16z⁴ − 20z² + 5)). This is a polynomial in u = z², so y = c0 + z·(a + b·u + d·u²), where the coefficients a, b, d are combinations of c1, c3, c5. Those combinations need plaintext-plaintext arithmetic, which isn't allowed. So I keep the ciphertext operations instead.

T3 = 4z³ − 3z and T5 = 16z⁵ − 20z³ + 5z, so y = c0 + c1 z + c3 T3 + c5 T5.

Direct evaluation in powers of u:
- z = x·alpha (1 pt mul, level 1).
- u = z² (1 ct mul, level 2).
- Coefficient mixing needs plaintext products such as c3·4, which cannot be built from plaintexts alone. Instead I use ciphertext-side scalings.
  - P = c5·u, which is a pt mul.
  - Q = (P + c3') ... 

Constants such as 4 can't be created. Via ciphertext doubling they are fine, though: 4·z² is u+u+u+u, and 16·u² is repeated doubling by addition, which is cheap.

Write y − c0 = z·[c1 − 3c3 + 5c5 + u(4c3 − 20c5) + 16c5·u²].
Let w = 4u (adds). Then:
- 16u² = w·w/1, which needs a ct mul, or equivalently 4w·u.
- Inner = c1 − 3c3 + 5c5 + (c3 − 5c5)·w + c5·w·w/1.

Define the ciphertext s = w·w, which is 16u². Then c5·s costs a pt mul and a ct mul. Alternatively, inner = A + w·(B + c5·w) with B = c3 − 5c5, which are ciphertext-side quantities.

Build the ciphertext constants from plaintext ones via ciphertext arithmetic:
- c3 − 5c5 can't be formed as plaintext-plaintext. Instead use c3·one-like terms on ciphertext: (w·c3) − 5·(w·c5), where 5·t = t+t+t+t+t, all adds.

So with w = 4u:
- m5 = w·c5 (pt mul) and m3 = w·c3 (pt mul).
- B·w = w·(c3 − 5c5)·w, which is a ct mul on w. Cheaper: w·(m3 − 5·m5) is 1 ct mul.
- Inner = c1 − 3c3 + 5c5 + w·(m3 − 5·m5 + c5·w)... 

Final structure:
- z = x·alpha
- u = z·z
- w = 4u (adds)
- m3 = w·c3 and m5 = w·c5, both at level 3
- r = m3 − 5·m5 + w·m5 = m3 + m5·(w − 5)... but subtracting 5 needs a constant, so skip it and use the ct mul w·m5 instead.
- Then I need the terms c1 z, c3 z, c5 z and the product z·r (depth 4 total).
- Constants: K = c1 − 3c3 + 5c5 is formed as z·c1 − 3(z·c3) + 5(z·c5) with 3 pt muls.
- y = K_z + z·r + c0.

The result has 3 ct muls (u, w·m5, z·r) and several pt muls. That is fewer ct muls than the baseline's 4. Scale mismatches are handled by CHEHAB through level alignment.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Ciphertext z = x * alpha;
  Ciphertext u = z * z;
  Ciphertext u2 = u + u;
  Ciphertext w = u2 + u2;
  Ciphertext m3 = w * c3;
  Ciphertext m5 = w * c5;
  Ciphertext m5x2 = m5 + m5;
  Ciphertext m5x4 = m5x2 + m5x2;
  Ciphertext m5x5 = m5x4 + m5;
  Ciphertext r = (m3 - m5x5) + w * m5;
  Ciphertext zc1 = z * c1;
  Ciphertext zc3 = z * c3;
  Ciphertext zc5 = z * c5;
  Ciphertext zc3x3 = (zc3 + zc3) + zc3;
  Ciphertext zc5x2 = zc5 + zc5;
  Ciphertext zc5x5 = (zc5x2 + zc5x2) + zc5;
  Ciphertext K = (zc1 - zc3x3) + zc5x5;
  Ciphertext y = K + z * r;
  y = y + c0;
  y.set_output("y");
}
```

I haven't compiled this or checked it numerically, and my derivation of r above was loose, so verify correctness from the result.