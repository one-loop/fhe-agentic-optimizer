The kernel is already at its floor: one ciphertext-plaintext multiply and one rescale, with no rotations, relinearization, or alignment. Timings across the three attempts (9.19, 8.92, 9.34 ms) are the same operation measured with noise, so there's nothing left to gain from restructuring. I'm resubmitting the best-measured version.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```