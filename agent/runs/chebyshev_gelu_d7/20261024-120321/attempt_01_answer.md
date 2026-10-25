Idea: the polynomial uses only T1, T2, T4, T6 (c3, c5, c7 are dropped), so it is a polynomial in z with even terms plus the linear term. Writing T2, T4 and T6 in terms of w = z², the even part is a cubic polynomial in w, and the odd part is c1·z.

T2 = 2w − 1
T4 = 8w² − 8w + 1
T6 = 32w³ − 48w² + 18w − 1

The even part is c0 − c2 + c4 − c6 + w(2c2 − 8c4 + 18c6) + w²(8c4 − 48c6) + w³·32c6.

This needs coefficient combinations that are computed from plaintext inputs. Plaintext-plaintext arithmetic is not allowed, so I would have to fold the coefficients into ciphertext operations instead.

Alternative: use u = x² directly, with w = u·α²; this still needs α² as a plaintext product. A cleaner route is to compute everything from ciphertexts and apply the plaintext coefficients by multiplication:

- z = x·alpha (ct×pt)
- w = z·z (ct×ct)
- w2 = w·w (ct×ct)
- w3 = w2·w (ct×ct)

Combining the terms with plaintext coefficients would require the combined coefficients, which I can't make. So I use the T basis but cut the multiplications down:

- T2 = 2w − one
- T4 = 2·T2² − one (ct×ct)
- T6 = 2·T2·T4 − T2 (ct×ct)

That gives 4 ct×ct multiplies: z², T2², T2·T4, plus the initial scaling, which is a plaintext multiply. Depth is z (1), w (2), T4 (3), T6 (4), then the coefficient multiply (5). To lower the depth and the scale-mismatch risk, I can apply the coefficients early:

- c6·T6 = 2·(c6·T2)·T4 − c6·T2, where c6·T2 is computed alongside T4, so the depth stays at 4.
- The result is c1·z + c2·T2 + c4·T4 + c6·T6 + c0.

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
  Ciphertext w = z * z;
  Ciphertext t2 = w + w - one;
  Ciphertext s = t2 * t2;
  Ciphertext t4 = s + s - one;
  Ciphertext c6t2 = t2 * c6;
  Ciphertext p = c6t2 * t4;
  Ciphertext term6 = p + p - c6t2;
  Ciphertext term1 = z * c1;
  Ciphertext term2 = t2 * c2;
  Ciphertext term4 = t4 * c4;
  Ciphertext result = term1 + term2;
  result = result + term4;
  result = result + term6;
  result = result + c0;
  result.set_output("y");
}
```