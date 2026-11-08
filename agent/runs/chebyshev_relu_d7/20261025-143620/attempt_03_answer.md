I could not find a structurally cheaper circuit than attempt 2, so I'm resubmitting it unchanged. It evaluates `y` through `s = z²`, `T2 = 2s - 1` and `T4 = 2·T2² - 1`. The `c6·T6` and `c4·T4` terms are merged into one product, `(2·c6·T2 + c4)·T4`.

Further savings would need combined coefficients such as `c2 - c6`, which would be plaintext-plaintext arithmetic, and that is not allowed. The other rearrangements I tried (expanding `T4` into `q`, splitting `c6·T2` through `s`) only swap one plaintext add for another.

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