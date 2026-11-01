I have no way to make this cheaper. The kernel needs one ciphertext-plaintext multiply and one rescale, and the baseline already has exactly that. The one variant I tried with a different circuit (attempt 5) was twice as slow. Without changing the computation, I'm submitting the baseline kernel again.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```