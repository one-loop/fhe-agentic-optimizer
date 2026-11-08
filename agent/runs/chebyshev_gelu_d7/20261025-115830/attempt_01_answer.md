Idea: the polynomial is even/odd mixed: y = c0 + c1 z + c2 T2 + c4 T4 + c6 T6. Write it as a polynomial in u = z², plus an odd term c1·z.

T2 = 2u − 1
T4 = 8u² − 8u + 1
T6 = 32u³ − 48u² + 18u − 1

So the even part is E(u) = a0 + a1 u + a2 u² + a3 u³, where the a's are combinations of the c's. Computing those combinations would need plaintext-plaintext arithmetic, which is not allowed. Instead I keep the Chebyshev terms but share the work:

- z = x·alpha (one plaintext multiplication, level 1)
- u = z² (one ct-ct multiplication, level 2)
- T2 = 2u − 1 (adds only)
- T4 = 2·T2² − 1 (one multiplication, level 3)
- T6 = 2·T2·T4 − T2 (one multiplication, level 4)

That is 3 ct-ct multiplications instead of 5. The terms are c1·z, c2·T2, c4·T4 and c6·T6, with plaintext multiplications at differing levels. The depth is 4 for T6, plus 1 for c6, so 5 in total.

Further reduction: use z² = x²·alpha², but alpha² is not available without a plaintext-plaintext multiplication. Alternatively, (x·alpha)·(x·alpha) is already one plaintext multiplication plus a square. Keeping z = x·alpha is fine.

The scales should be consistent, because each term meets the others at a different level and CHEHAB aligns them.

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
  Ciphertext u = square(z);
  Ciphertext t2 = u + u - one;
  Ciphertext t2s = square(t2);
  Ciphertext t4 = t2s + t2s - one;
  Ciphertext m = t2 * t4;
  Ciphertext t6 = m + m - t2;
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result + t4 * c4;
  result = result + t6 * c6;
  result = result + c0;
  result.set_output("y");
}
```