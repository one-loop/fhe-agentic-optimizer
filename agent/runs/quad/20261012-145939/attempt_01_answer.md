The baseline already uses the minimum possible work: one ciphertext-ciphertext multiply, one relinearization, and one rescale, at depth 1. No rotations or plaintext operations are needed, and constants aren't allowed anyway. I'm submitting `square(x)` as an equivalent spelling. It may compile to the same graph, so I don't expect a meaningful speedup.

```cpp
// y = x^2 with a single ciphertext squaring (depth 1, no rotations).

void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```