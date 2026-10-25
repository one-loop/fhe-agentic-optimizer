Seven attempts have confirmed that x² costs one multiply, one relinearization and one rescale in this DSL. The timings (64.6–67.5 ms) are noise around that cost. I found no valid rewrite that removes any of those operations, since every alternative needs a constant, an extra multiply, or a rotation.

For the last attempt I'm submitting the baseline form again, which is the best measured result.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = x * x;
  y.set_output("y");
}
```