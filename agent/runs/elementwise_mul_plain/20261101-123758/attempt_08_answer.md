This final attempt resubmits the baseline, so the best result for this task stays at 7.06 ms. Its circuit is one ct-pt multiply plus the automatic rescale, which is the minimum for `y = x0 * p`. Since `p` is a run-time plaintext vector and CKKS allows no constants, that multiply cannot be removed. Any structurally different kernel would add operations and be slower.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```