The kernel is already minimal: one ciphertext-plaintext multiply and one rescale, with no rotations, relinearization or level alignment. The 13.15 ms vs 13.43 ms difference is run-to-run noise, since the op graph is identical. I see no legal rewrite that does fewer multiplications, so I'm resubmitting the same kernel.

```cpp
// Elementwise multiply by a plaintext vector: minimal form (one ct x pt multiply, depth 1).
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```