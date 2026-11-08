Idea: cut the multiplications further by restructuring.

Current: z = x·alpha (pt), s = z² (ct), d = s·c5 (pt), f = s·c3 (pt), e = d·s (ct), y = z·P (ct). That is 3 ct-ct and 3 pt multiplies, with depth 5.

Alternative: skip z and work with x directly. Writing z = alpha·x, y = z·P(z²), so z² = alpha²·x². Only a single alpha plaintext is available, so alpha² would need a ct-pt multiply anyway. Instead:
- s = z², with z = x·alpha (1 pt mul, 1 square).
- Let g = c5·s + c3·... but a Horner form needs P = s·(A·s + B) + C, with A = 16c5, B = 4c3 − 20c5 and C = c1 − 3c3 + 5c5.
- Form q = (c5·s) as d (pt mul), then h = 16d + (4c3 − 20c5)·s. The s-linear part uses f = c3·s, giving h = 16d + 4f − 20d = 4f − 4d. So h = 4(f − d), which saves the doublings.
- Then P = s·(h/s...)? Check: P = 16c5 s² + (4c3 − 20c5)s + C = s·(16d' ...) with d' = c5·s, giving 16 d'·s... Alternatively, Horner: P = s·(16c5·s + 4c3 − 20c5) + C. The inner term needs c5·s, which is d, so inner = 16d + 4c3 − 20c5. That is a plaintext add on a ciphertext, with 4c3 and 20c5 made via repeated plaintext adds, which is costly.
- Better: P = s·(16d − 4·c5·... ) no, I keep the earlier form but cut the adds: e = d·s, then P = 16e + 4(f − d) + C, where 4(f − d) = 4f − 4d (this replaces the d16 and d4 chain with one subtraction and two doublings).

This keeps the same multiplications but removes about 6 adds. The circuit differs, and the saving is small but real.

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
  Ciphertext e2 = e + e;
  Ciphertext e4 = e2 + e2;
  Ciphertext e8 = e4 + e4;
  Ciphertext e16 = e8 + e8;
  Ciphertext g = f - d;
  Ciphertext g2 = g + g;
  Ciphertext g4 = g2 + g2;
  Ciphertext P = e16 + g4;
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

Wait, I need to check the math: 4f − 20d is not 4(f − d). Correct: 4f − 20d = 4(f − 5d). So I must fix this.

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
  // 16e - 20d + 4f = 4*(4e - 5d + f)
  Ciphertext e2 = e + e;
  Ciphertext e4 = e2 + e2;
  Ciphertext d2 = d + d;
  Ciphertext d4 = d2 + d2;
  Ciphertext d5 = d4 + d;
  Ciphertext inner = e4 - d5 + f;
  Ciphertext i2 = inner + inner;
  Ciphertext P = i2 + i2;
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