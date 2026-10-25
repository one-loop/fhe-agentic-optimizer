The kernel is still one ciphertext-plaintext multiply plus its rescale, which is the minimum for y = x0 * s. The spread across runs (8.80, 8.89, 9.46, 9.01 ms) is noise on identical programs. I can't find a cheaper equivalent in the DSL, since the scalar exists only as a plaintext input and every other route needs more operations. I'm resubmitting the best kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```