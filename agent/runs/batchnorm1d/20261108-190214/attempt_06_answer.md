This attempt again produced the baseline circuit, which is the expected outcome. As in attempts 1–4, the baseline already uses only one multiply by A, one rescale and one add of B, and no cheaper circuit exists. The baseline remains the final answer.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```