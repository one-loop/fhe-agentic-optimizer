I haven't found a cheaper circuit than attempt 1. Every alternative I tried needs a third plaintext multiplication or an extra level. I'm resubmitting the best structure with the doubling moved from m to z (m·(z+z) + c1·z). The ciphertext-ciphertext product then has operands at different levels, so this variant is probably slower. The real best remains attempt 1.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext m = z * c2;
  Ciphertext s = m + m;
  s = s + c1;
  Ciphertext p = s * z;
  Ciphertext y = (p + c0) - c2;
  y.set_output("y");
}
```