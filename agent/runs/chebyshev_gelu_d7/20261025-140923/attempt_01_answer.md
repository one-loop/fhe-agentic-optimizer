Idea: the polynomial only needs even terms c2, c4, c6, c0 plus the odd term c1·z. With u = z², the even part is a polynomial in u, and the odd part is just c1·z.

Expand in the monomial basis, with s = z²:
- T2 = 2s − 1
- T4 = 8s² − 8s + 1
- T6 = 32s³ − 48s² + 18s − 1

Then y = c0 + c1 z + c2 T2 + c4 T4 + c6 T6 = A + B s + C s² + D s³, where:
- A = c0 − c2 + c4 − c6
- B = 2c2 − 8c4 + 18c6
- C = 8c4 − 48c6
- D = 32c6

Computing these coefficients would need plaintext-plaintext arithmetic, which is not allowed. Every operation needs a ciphertext operand, so I can't build A–D directly from the plaintext inputs.

Alternative: keep the Chebyshev form but cut the multiplications.
- z = x·alpha (one plaintext multiplication).
- s = z·z (one ct-ct multiplication).
- T2 = s + s − one.
- T4 = 2·T2² − 1 (one ct-ct multiplication, via square).
- T6 = 2·T2·T4 − T2 (one ct-ct multiplication).

That is 3 ct-ct multiplications instead of 5. The depth is z (1), s (2), T2 (2), T4 (3), T6 (4).

Terms:
- c1·z: z is at level 1, and this is a plaintext multiplication.
- c2·T2, c4·T4 and c6·T6 are plaintext multiplications.

To lower the cost I could use T2 = 2s − 1 directly with s = z², but mixing scales is handled by CHEHAB. The subtractions of `one` are plaintext subtractions, which are fine.

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
  Ciphertext p = t2 * t4;
  Ciphertext t6 = p + p - t2;
  Ciphertext result = z * c1 + t2 * c2 + t4 * c4 + t6 * c6 + c0;
  result.set_output("y");
}
```