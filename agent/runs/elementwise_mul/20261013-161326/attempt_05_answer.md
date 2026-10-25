I'm not changing the kernel. Attempt 4 gave the same circuit as every previous attempt (one ciphertext-ciphertext multiply, one relinearization, one rescale, depth 1) and timed at 67.6 ms, inside the same 64.8 to 68.2 ms noise band. The kernel cannot get cheaper: any valid computation of x0 * x1 needs at least one ciphertext-ciphertext multiply, and the squaring identities need two. The best measured version is attempt 1, so I'm returning that.

```cpp
// Elementwise product: one ciphertext x ciphertext multiply (depth 1, relinearized, rescaled).
// This is the minimal circuit for x0 * x1 in CKKS.

void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```