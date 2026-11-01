The new kernel computes the same Chebyshev sum in fewer, cheaper steps. It replaces the baseline's 5 ciphertext multiplications with 3, uses 4 plaintext multiplications instead of 5, and cuts the multiplicative depth from 7 to 4.

1. **Build only even terms from `u = T2(z)`.** Every non-negligible term except `c1·T1` is even, so the kernel works with `u = T2(z) = 2z² - 1`.
   - `T4 = 2u² - 1`
   - `T6 = 2u·T4 - u`
2. **Factor the T4 and T6 terms.** The two terms combine into one product:
   - `c4·T4 + c6·T6 = T4·(c4 + 2·c6·u) - c6·u`
   - With `s = c6·u`, the output becomes `y = c0 + c1·z + c2·u - s + T4·(s + s + c4)`.
3. **Operation count.** This needs 3 ciphertext multiplications: `z²`, `u²` and `T4·v`. It needs 4 plaintext multiplications: by `alpha`, `c1`, `c2` and `c6`.
4. **Scales stay compatible.** Terms that are added at the same level were produced by the same pattern of multiplications. For example, `c2·u` and `s` are both at level 3 and both come from one plaintext multiplication of `u`.

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
  Ciphertext u = w + w - one;          // T2
  Ciphertext u2 = u * u;
  Ciphertext t4 = u2 + u2 - one;       // T4
  Ciphertext s = u * c6;               // c6*T2
  Ciphertext v = s + s + c4;           // c4 + 2 c6 T2
  Ciphertext p = t4 * v;               // c4 T4 + c6 (T6 + T2)
  Ciphertext lin = z * c1;
  Ciphertext q = u * c2;
  Ciphertext result = p + (q - s) + lin + c0;
  result.set_output("y");
}
```