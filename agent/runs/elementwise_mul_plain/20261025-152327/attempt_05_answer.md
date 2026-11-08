I still have no cheaper circuit. The task is one ct-pt multiply plus its rescale, and every alternative I can write under the CKKS rules adds operations. I'm submitting the same kernel again, so it will again be reported as identical to the baseline.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```