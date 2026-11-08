I have no cheaper circuit to offer, and I'm not going to pretend otherwise. The task is a single ct-pt multiply plus its rescale. That is the minimum for x0 * p, and every variant I can build under the CKKS rules only adds operations. Four resubmissions of the baseline have all been judged identical to it, so I'm submitting it once more.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```