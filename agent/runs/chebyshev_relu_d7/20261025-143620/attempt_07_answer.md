I'm resubmitting attempt 2's kernel. I could not find a cheaper structure.

- **Multiplication count:** the cubic in `s = z²` needs `s`, `s²` and one more product, so three ciphertext multiplications are the minimum. A Horner form in `T2` needs an extra level for the same count.
- **Plaintext multiplications:** the remaining ones (`z·alpha`, `c1·z`, `c2·T2`, `c6·T2`) can't be merged, because that would require combining coefficients, which is plaintext-plaintext arithmetic.

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
  Ciphertext a2 = a + a + c4;
  Ciphertext p = a2 * t4;
  Ciphertext u = t2 * c2;
  Ciphertext w = z * c1;
  Ciphertext lower = u - a;
  Ciphertext result = p + lower;
  result = result + w;
  result = result + c0;
  result.set_output("y");
}
```