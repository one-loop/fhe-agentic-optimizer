I don't see a cheaper circuit for this task. The computation is one plaintext-vector multiply plus its rescale. Every alternative I can express under the CKKS rules adds operations:

- Constants are forbidden, so I can't fold in a factor like `(x0+x0)*(p/2)`.
- Plaintext-plaintext arithmetic is forbidden, so I can't precombine anything with `p`.
- A ct-ct multiply, a rotation or a split into halves costs more than the single ct-pt multiply.
- Padding the circuit with extra adds or mod switches only makes it slower.

I'm therefore resubmitting the minimal form. It will likely be reported as identical to the baseline.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```