The baseline is still the cheapest circuit I can find, and I have no different one to try. One ciphertext-plaintext multiplication with its rescale is the minimum for `x0 * s` with a run-time `s`. The result of this last attempt would be the same as the previous ones, so I'm submitting the baseline kernel one final time.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```