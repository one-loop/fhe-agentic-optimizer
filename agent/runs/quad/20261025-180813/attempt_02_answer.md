The baseline already needs only one ciphertext multiplication, which is the minimum for x². Without numeric constants I can't scale or halve a rewritten form, so no cheaper algebraic form exists. The `square` variant was no faster either.

For this attempt I'll build x² from `(-x) * x`, negated back. It has a different operation count (extra negations), but I don't expect it to beat the baseline. It is mainly a probe of whether the multiplication path matters.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext nx = -x;
  Ciphertext y = -(nx * x);
  y.set_output("y");
}
```