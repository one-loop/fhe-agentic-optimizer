The kernel has stayed at one multiply, one relinearization and one rescale, and the timings (64.6–67.5 ms) are noise around that cost. I see no valid way to remove any of those operations. Every algebraic alternative needs a constant or an extra multiply, so I'm submitting the baseline form again rather than guess at a variant I expect to be worse.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = x * x;
  y.set_output("y");
}
```