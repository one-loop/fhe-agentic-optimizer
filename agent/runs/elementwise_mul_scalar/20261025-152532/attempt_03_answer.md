I still see no valid circuit cheaper than the baseline. Every alternative I can build from the allowed operations adds work: extra adds, extra multiplies, or alignments. Without numeric constants I can't rescale `s` or `x0` at compile time to cancel any of it. Rather than submit a slower circuit, I'm keeping the minimal form: one plaintext multiply with its rescale.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```