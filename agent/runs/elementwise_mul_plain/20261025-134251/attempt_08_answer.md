I still have no cheaper circuit. The baseline is one ciphertext-plaintext multiply with one rescale, and the output depends on both inputs, so that multiply can't be dropped. Every other form I've found adds operations, as attempt 5 showed. I'm submitting the baseline kernel one last time.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```