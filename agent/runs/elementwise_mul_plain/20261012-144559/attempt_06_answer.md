I'm stopping here and submitting the baseline kernel unchanged. Across all six runs it has been correct, with timings between 7.03 and 7.06 ms. It performs one ciphertext-plaintext multiply, the minimum the task requires, and no rotations, ciphertext-ciphertext multiplies, or extra level consumption. The spread is run-to-run noise, so further attempts would not produce a meaningful speedup.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```