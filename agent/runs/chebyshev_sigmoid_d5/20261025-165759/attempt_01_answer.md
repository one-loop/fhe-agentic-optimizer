Approach: write the odd polynomial directly in z, with the coefficients folded in as plaintext multiplies.

y = c0 + c1 z + c3 T3 + c5 T5, with T3 = 4z³ − 3z and T5 = 16z⁵ − 20z³ + 5z.
So y = c0 + z(a1 + a3 z² + a5 z⁴), where a1 = c1 − 3c3 + 5c5, a3 = 4c3 − 20c5, a5 = 16c5.

Plaintext-plaintext arithmetic is not allowed, so I can't precompute the a's. Instead I keep the c's as plaintext multiplies on ciphertexts:

- Let u = z² and w = x·alpha.
- y = c0 + z·(c1 − 3c3 + 5c5) + ... would need plaintext combinations, so I use ciphertext-side sums instead.
- Using only ciphertext additions, 3c3 = c3 + c3 + c3 (plaintext additions are fine as long as a ciphertext is involved).

Cheaper structure: let s = z² (one ct-ct square, z = x·alpha at level 1, so s lives at level 2).
- P = c5·(16 s² − 20 s + 5) + c3·(4 s − 3) + c1, evaluated in s, then y = z·P + c0.
- The T-polynomials can be built from s without plaintext constants, except for the "one" plaintext (the constant term).
- Q5 = 16 s² − 20 s + 5 and Q3 = 4 s − 3 each need constants. Instead I group by s: P = s²·(16 c5) + s·(4 c3 − 20 c5) + (c1 − 3 c3 + 5 c5).
- Compute d = c5·s (ct-pt multiply), then e = d·s (ct-ct multiply, giving c5 s²), and f = c3·s. Then 16 c5 s² is 4 doublings of e.
- Term s: 4 c3 s − 20 c5 s = 4f − 20d, built from additions of f and d (20d = 16d + 4d, using doublings).
- Constant: c1 − 3c3 + 5c5 needs c1, c3, c5 as plaintexts. Adding a plaintext to a ciphertext is allowed, so I add them one at a time to the ciphertext: P = ... + c1 − c3 − c3 − c3 + c5 ×5. Each is a plaintext add at ~2.5 ms.
- Finally y = z·P + c0, which costs one more ct-ct multiply.

The multiplications are: z (pt), s (ct-ct), d and f (pt), e (ct-ct), and z·P (ct-ct), so 3 ct-ct and 3 pt multiplies, versus 4 and 4 in the baseline. Depth is z(1), s(2), d(3), e(4), z·P(5).

Alternatively, I could fold alpha into s directly, using s = (x·x)·alpha², but that would need alpha² as a plaintext. I stay with z.

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
  Ciphertext s = square(z);
  Ciphertext d = s * c5;
  Ciphertext f = s * c3;
  Ciphertext e = d * s;
  // 16 e
  Ciphertext e2 = e + e;
  Ciphertext e4 = e2 + e2;
  Ciphertext e8 = e4 + e4;
  Ciphertext e16 = e8 + e8;
  // 4f - 20d = 4f - 16d - 4d
  Ciphertext d2 = d + d;
  Ciphertext d4 = d2 + d2;
  Ciphertext d8 = d4 + d4;
  Ciphertext d16 = d8 + d8;
  Ciphertext f2 = f + f;
  Ciphertext f4 = f2 + f2;
  Ciphertext lin = f4 - d16 - d4;
  Ciphertext P = e16 + lin;
  // constant: c1 - 3c3 + 5c5
  P = P + c1;
  P = P - c3;
  P = P - c3;
  P = P - c3;
  P = P + c5;
  P = P + c5;
  P = P + c5;
  P = P + c5;
  P = P + c5;
  Ciphertext y = z * P;
  y = y + c0;
  y.set_output("y");
}
```