This is my last attempt, and I still have no cheaper circuit. Multiplying by a run-time scalar needs the one plaintext multiply and its rescale. Nothing else in the baseline can be dropped, and anything I add would only make it slower.

I'm submitting the minimal kernel again, so the baseline remains the best.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```