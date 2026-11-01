The `square` version is essentially the same as the baseline: 31.01 ms against 31.41 ms, which is within run-to-run noise. The single ciphertext squaring with relinearization and rescale is the floor for this function. No rotation, plaintext multiply, or depth reduction can replace it, so I'm keeping the same kernel as the best result.

```cpp
// y = x^2: one ciphertext squaring (relinearize + rescale), depth 1.
// This is the minimum possible work for a quadratic activation.

void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```