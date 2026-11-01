I still can't find a cheaper circuit. Rather than resubmit the identical kernel a fifth time, I'll try a structurally different one to see how CHEHAB lowers it: `(x0 + x0) * p - x0 * p`. It should be slower, because it has two plaintext multiplies, and I expect it to lose to the baseline. The result is still informative, since the baseline remains the best kernel regardless.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext d = x0 + x0;
  Ciphertext y = d * p - x0 * p;
  y.set_output("y");
}
```