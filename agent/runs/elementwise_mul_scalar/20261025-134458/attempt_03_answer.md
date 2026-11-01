I don't see a cheaper circuit. The baseline is already one scalar ct-pt multiply plus one rescale. Attempt 2 showed that splitting the product only adds multiplies and rescales. Without numeric constants, and with `s` available only as a plaintext input, every rewrite I can think of either adds operations or gets simplified back to the baseline. I'm resubmitting the minimal form, which won't count as an improvement.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = s * x0;
  y.set_output("y");
}
```