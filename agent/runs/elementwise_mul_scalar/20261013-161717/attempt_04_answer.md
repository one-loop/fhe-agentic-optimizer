The kernel is already at the minimum: one ciphertext-plaintext multiply and one rescale, with no rotations, relinearization, or level alignment. The four runs (9.19, 8.92, 9.34, 9.10 ms) differ only by noise, and nothing in the DSL offers a cheaper way to compute `x0 * s`. I'm resubmitting the same kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```