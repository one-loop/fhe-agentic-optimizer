There is still no cheaper circuit than one ciphertext multiplication. I'll try one last structural variant that keeps a single multiply but adds an explicit `x - x` zero term, to see how CHEHAB normalizes it. It can't be faster than the baseline in principle, so this is only a probe.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext z = x - x;
  Ciphertext y = x * x + z;
  y.set_output("y");
}
```